#include "VSyncFix.h"

// Addons
#include "Addons/ConfigFile.h"
#include "Addons/Hook.h"
#include "Addons/tConsole.h"

// Pl2
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#include <cstdint>
#include <d3d9.h>

// Win10 1803+; declared here so the module still builds against older SDK headers
#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

uint64_t Displ_WaitForFrameLock_Old;
void __cdecl HOOK_Displ_WaitForFrameLock(int InVBlankCount);

namespace
{
	// [Fixes] ApplyVSyncFix in gf2asi_settings.ini. Default on: the fix only
	// corrects a present interval the game gets wrong in every configuration.
	bool bApplyVSyncFix = true;

	void LoadTuning()
	{
		bApplyVSyncFix = Mod::Config::SettingsFile().GetBool(L"Fixes", L"ApplyVSyncFix", bApplyVSyncFix);

		tConsole::fPrintf("Wants VSync fix: %u", bApplyVSyncFix);
	}

	// Game globals (Steam exe)

	// Present parameters the renderer hands to CreateDevice and Reset. Rebuilt
	// by Displ_BuildPresentParams on every mode change, so reading Windowed and
	// PresentationInterval here always reflects the live device.
	hook::Type<D3DPRESENT_PARAMETERS> Dx9PresentParams = hook::Type<D3DPRESENT_PARAMETERS>(0x11D9188);

	// Refresh rate picked in the video options, in Hz. Only reaches
	// FullScreen_RefreshRateInHz in fullscreen; windowed leaves that field 0.
	hook::Type<uint16_t> SelectedRefreshRate = hook::Type<uint16_t>(0x1205760);

	// The immediate operands of the two stores that put D3DPRESENT_INTERVAL_TWO
	// into the present parameters, in the fullscreen VSync branch of
	// Displ_BuildPresentParams and again in Displ_BuildFallbackPresentParams,
	// which Displ_CreateDevice uses when the first one fails. Overwriting them
	// with D3DPRESENT_INTERVAL_ONE is the whole fullscreen fix: the surrounding
	// capability tests are left alone, they just no longer select a half rate
	// interval when they pass.
	constexpr uintptr_t PRESENT_INTERVAL_IMMEDIATES[] = { 0x6075E2, 0x608C21 };

	// Displ_WaitForFrameLock, the software VSync used in windowed mode
	constexpr uintptr_t DISPL_WAITFORFRAMELOCK = 0x608CA0;

	// Fall back to this when neither the desktop nor the game can name a
	// refresh rate, and clamp anything implausible into range.
	constexpr double DEFAULT_REFRESH_HZ = 60.0;
	constexpr double MIN_REFRESH_HZ = 20.0;
	constexpr double MAX_REFRESH_HZ = 1000.0;

	// Hand the last millisecond back to a spin loop; waitable timers are only
	// accurate to roughly that, and overshooting drops a frame.
	constexpr double SPIN_MARGIN_MS = 1.0;

	// How many paced frames a cached refresh rate survives before it is
	// re-queried, so a desktop mode change is picked up without asking the
	// display driver every single frame.
	constexpr uint32_t REFRESH_CACHE_FRAMES = 600;

	// Set once StaticApplyHooks has actually patched and detoured, so the
	// detour falls through to stock behaviour if installation was skipped.
	bool bFixInstalled = false;

	// QPC ticks per millisecond, cached on first use
	double TicksPerMs = 0.0;

	// QPC timestamp of the last paced present; 0 until the first one
	int64_t LastPresentTicks = 0;

	// Cached windowed frame period and its age, see REFRESH_CACHE_FRAMES
	double CachedPeriodMs = 0.0;
	uint32_t CachedPeriodAge = 0;

	// High resolution waitable timer, so pacing does not have to spin a whole
	// core. Null on older Windows, where the spin loop is used on its own,
	// which is what the game did for every frame anyway.
	HANDLE HighResTimer = nullptr;

	bool IsWindowed()
	{
		return Dx9PresentParams->Windowed != FALSE;
	}

	// Refresh rate the windowed swap chain is effectively limited by. A
	// composited window presents at the desktop rate, so that is preferred over
	// the game selection, which only applies to fullscreen.
	double QueryRefreshHz()
	{
		DEVMODEW DisplayMode = {};
		DisplayMode.dmSize = sizeof(DisplayMode);

		if (EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &DisplayMode)
			&& (DisplayMode.dmFields & DM_DISPLAYFREQUENCY) != 0
			&& DisplayMode.dmDisplayFrequency > 1)
		{
			return static_cast<double>(DisplayMode.dmDisplayFrequency);
		}

		const uint16_t GameRefreshRate = SelectedRefreshRate.get();
		if (GameRefreshRate > 1)
		{
			return static_cast<double>(GameRefreshRate);
		}

		return DEFAULT_REFRESH_HZ;
	}

	double GetWindowedFramePeriodMs()
	{
		if (CachedPeriodMs > 0.0 && CachedPeriodAge < REFRESH_CACHE_FRAMES)
		{
			++CachedPeriodAge;
			return CachedPeriodMs;
		}

		double RefreshHz = QueryRefreshHz();
		if (RefreshHz < MIN_REFRESH_HZ || RefreshHz > MAX_REFRESH_HZ)
		{
			RefreshHz = DEFAULT_REFRESH_HZ;
		}

		CachedPeriodMs = 1000.0 / RefreshHz;
		CachedPeriodAge = 0;
		return CachedPeriodMs;
	}

	// Blocks until the QPC deadline, sleeping through the bulk of the wait and
	// spinning out the last SPIN_MARGIN_MS.
	void WaitUntil(const int64_t InDeadlineTicks)
	{
		for (;;)
		{
			LARGE_INTEGER Now;
			QueryPerformanceCounter(&Now);

			const int64_t RemainingTicks = InDeadlineTicks - Now.QuadPart;
			if (RemainingTicks <= 0)
			{
				return;
			}

			const double RemainingMs = static_cast<double>(RemainingTicks) / TicksPerMs;
			if (HighResTimer && RemainingMs > SPIN_MARGIN_MS)
			{
				// Negative due time is relative, in 100 ns units
				LARGE_INTEGER DueTime;
				DueTime.QuadPart = -static_cast<int64_t>((RemainingMs - SPIN_MARGIN_MS) * 10000.0);

				if (SetWaitableTimer(HighResTimer, &DueTime, 0, nullptr, nullptr, FALSE))
				{
					WaitForSingleObject(HighResTimer, INFINITE);
					continue;
				}
			}

			// Yield rather than sleep: the default timer resolution would
			// overshoot a whole frame.
			Sleep(0);
		}
	}

	// Windowed replacement for the stock frame lock. Paces to a single real
	// refresh period instead of two hardcoded 60 Hz ones.
	void PaceWindowedFrame()
	{
		if (TicksPerMs <= 0.0)
		{
			LARGE_INTEGER Frequency;
			if (!QueryPerformanceFrequency(&Frequency) || Frequency.QuadPart <= 0)
			{
				return;
			}

			TicksPerMs = static_cast<double>(Frequency.QuadPart) / 1000.0;
		}

		LARGE_INTEGER Now;
		QueryPerformanceCounter(&Now);

		if (LastPresentTicks != 0)
		{
			const int64_t PeriodTicks = static_cast<int64_t>(GetWindowedFramePeriodMs() * TicksPerMs);
			const int64_t DeadlineTicks = LastPresentTicks + PeriodTicks;

			// Only wait when the deadline is still ahead. A long stall, a load
			// or an alt-tab, leaves it in the past, and paying that back over
			// the following frames would just stutter.
			if (DeadlineTicks > Now.QuadPart)
			{
				WaitUntil(DeadlineTicks);
				QueryPerformanceCounter(&Now);
			}
		}

		LastPresentTicks = Now.QuadPart;
	}
}

/**
 * Called by Displ_SwapScreens once per frame, before Present, with the vblank
 * count the renderer wants to wait out (2 while VSync is on, and the call is
 * skipped entirely while it is off).
 *
 * Fullscreen no longer needs it at all: the corrected present interval means
 * the driver blocks in Present for exactly one refresh. Windowed still does,
 * because the windowed path always presents immediately, so it is paced here
 * against the real refresh rate.
 */
void __cdecl HOOK_Displ_WaitForFrameLock(int InVBlankCount)
{
	if (!bFixInstalled)
	{
		PLH::FnCast(Displ_WaitForFrameLock_Old, &HOOK_Displ_WaitForFrameLock)(InVBlankCount);
		return;
	}

	// Matches the stock gate: nothing to wait for while VSync is off
	if (InVBlankCount < 1)
	{
		return;
	}

	if (!IsWindowed())
	{
		// Hardware VSync is doing the work now. Drop the timestamp so a later
		// switch back to windowed does not pace against a stale one.
		LastPresentTicks = 0;
		return;
	}

	PaceWindowedFrame();
}

void Mod::VSyncFix::StaticApplyHooks()
{
	LoadTuning();

	if (!bApplyVSyncFix)
	{
		return;
	}

	// Both fullscreen VSync branches currently store D3DPRESENT_INTERVAL_TWO,
	// and nothing else in either branch writes the field, so swapping the value
	// they store is enough. It lands before the first CreateDevice as well as
	// every later Reset. Verify before writing, so a different build of the exe
	// gets a log line rather than a corrupted instruction.
	for (const uintptr_t IntervalAddress : PRESENT_INTERVAL_IMMEDIATES)
	{
		const DWORD PreviousInterval = *reinterpret_cast<DWORD*>(IntervalAddress);
		if (PreviousInterval != D3DPRESENT_INTERVAL_TWO)
		{
			tConsole::fPrintf("VSyncFix: unexpected present interval %u at 0x%X, skipping fix", PreviousInterval, IntervalAddress);
			return;
		}
	}

	for (const uintptr_t IntervalAddress : PRESENT_INTERVAL_IMMEDIATES)
	{
		if (!MemUtils::WriteMemory<DWORD>(IntervalAddress, D3DPRESENT_INTERVAL_ONE))
		{
			tConsole::fPrintf("VSyncFix: could not write present interval at 0x%X, skipping fix", IntervalAddress);
			return;
		}
	}

	HighResTimer = CreateWaitableTimerExW(nullptr, nullptr,
		CREATE_WAITABLE_TIMER_MANUAL_RESET | CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);

	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour200((char*)DISPL_WAITFORFRAMELOCK, (char*)&HOOK_Displ_WaitForFrameLock, &Displ_WaitForFrameLock_Old, dis);
	detour200.hook();

	bFixInstalled = true;

	tConsole::fPrintf("VSyncFix: fullscreen VSync now presents every refresh, windowed paced by timer (%s)",
		HighResTimer ? "high resolution" : "spin");
}
