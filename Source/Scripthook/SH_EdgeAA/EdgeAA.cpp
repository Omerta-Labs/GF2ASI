#include "EdgeAA.h"

// Addons
#include "Addons/ConfigFile.h"
#include "Addons/Hook.h"
#include "Addons/tConsole.h"

// Pl2
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#include <algorithm>

uint64_t ScreenEdgeAA_PrepareRender_Old;
int __cdecl HOOK_ScreenEdgeAA_PrepareRender();

namespace
{
	/**
	 * [EdgeAA] in gf2asi_settings.ini. The member initialisers are the
	 * defaults, so a key that is not in the file leaves that value alone.
	 */
	struct EdgeAATuning
	{
		// Force the EdgeAA post-process on every frame.
		// Default off until it has been tested enough for release.
		bool bEnable = false;

		// Edge blur strength (uploaded as g_TexelSize.z). The engine skips the
		// pass below 1/127; the shader was authored around values of ~1.0.
		float BlurWidth = 1.0f;

		// Distance in texels between edge-detect samples
		float SampleLength = 1.0f;

		// Edge cutoff, only used by the deferred-mode shader variant
		float Cutoff = 0.0f;

		// Edge detection threshold (g_BarrierAndWeights.x); engine default 0.8.
		// Lower values classify more pixels as edges to be smoothed.
		float Barrier = 0.8f;

		// Depth-difference weight (g_BarrierAndWeights.z); engine default ~7e-6.
		// Higher values make small depth discontinuities register as edges.
		float DepthWeight = 6.9907e-06f;

		// Use the deferred-AA shader variant (normal-buffer edge detect).
		// Experimental - the game data never enables this path on PC.
		bool bDeferredMode = false;
	};

	EdgeAATuning Tuning;

	void LoadTuning()
	{
		const Mod::ConfigFile& File = Mod::Config::SettingsFile();

		Tuning.bEnable = File.GetBool(L"EdgeAA", L"Enable", Tuning.bEnable);
		Tuning.BlurWidth = File.GetFloat(L"EdgeAA", L"BlurWidth", Tuning.BlurWidth);
		Tuning.SampleLength = File.GetFloat(L"EdgeAA", L"SampleLength", Tuning.SampleLength);
		Tuning.Cutoff = File.GetFloat(L"EdgeAA", L"Cutoff", Tuning.Cutoff);
		Tuning.Barrier = File.GetFloat(L"EdgeAA", L"Barrier", Tuning.Barrier);
		Tuning.DepthWeight = File.GetFloat(L"EdgeAA", L"DepthWeight", Tuning.DepthWeight);
		Tuning.bDeferredMode = File.GetBool(L"EdgeAA", L"DeferredMode", Tuning.bDeferredMode);

		tConsole::fPrintf("EdgeAA enhance: %u (BlurWidth=%.2f, SampleLength=%.2f, Barrier=%.2f, DepthWeight=%g, Deferred=%u)",
			Tuning.bEnable, Tuning.BlurWidth, Tuning.SampleLength, Tuning.Barrier, Tuning.DepthWeight, Tuning.bDeferredMode);
	}

	// EARS::Alchemy::ScreenEdgeAA globals (Steam exe)

	// Per-frame effect parameters: blur width, sample length, cutoff, unused.
	// Consumed by ScreenEdgeAA::RenderImmediate (or the combined post pass when
	// other effects are active) and zeroed afterwards, so they have to be
	// re-submitted every frame.
	hook::Type<float[4]> EdgeAAParams = hook::Type<float[4]>(0x1197D70);

	// Edge detection constants, uploaded as g_BarrierAndWeights: barrier
	// threshold in x, filter weights in y/z/w. Engine defaults are
	// { 0.8, 1.0, ~7e-6, 1.0 }.
	hook::Type<float[4]> EdgeAABarrierAndWeights = hook::Type<float[4]>(0x1197D80);

	// Parameter flags; 0x400000 selects the deferred-AA shader variant that
	// edge-detects against the normal buffer instead of only depth.
	hook::Type<uint32_t> EdgeAAFlags = hook::Type<uint32_t>(0x1197D90);

	// Post-FX chain skip flag for EdgeAA (index 16 of the per-effect disable
	// array). The quality-option apply sets it whenever QualityOptions.EdgeAA
	// is 0, and the chain calls neither PrepareRender nor Render while it is.
	hook::Type<uint8_t> EdgeAADisabled = hook::Type<uint8_t>(0x1194748);

	// The pass is skipped (Params::IsNull) when blur width or sample length
	// fall below 1/127, so enabled values are clamped to stay above it.
	constexpr float MIN_ACTIVE_PARAM = 1.0f / 127.0f;
}

/**
 * Called by the post-FX chain once per frame, after the game's screen effects
 * have submitted their parameters and before the chain picks its passes.
 * Overwrites the parameter block so the pass always runs with the ini tuning,
 * including on the (rare) frames where level data submitted its own values.
 */
int __cdecl HOOK_ScreenEdgeAA_PrepareRender()
{
	if (Tuning.bEnable)
	{
		EdgeAAParams[0] = std::max(Tuning.BlurWidth, MIN_ACTIVE_PARAM);
		EdgeAAParams[1] = std::max(Tuning.SampleLength, MIN_ACTIVE_PARAM);
		EdgeAAParams[2] = Tuning.Cutoff;

		EdgeAABarrierAndWeights[0] = Tuning.Barrier;
		EdgeAABarrierAndWeights[2] = Tuning.DepthWeight;

		if (Tuning.bDeferredMode)
		{
			EdgeAAFlags.set(EdgeAAFlags.get() | 0x400000u);
		}
	}

	return PLH::FnCast(ScreenEdgeAA_PrepareRender_Old, &HOOK_ScreenEdgeAA_PrepareRender)();
}

void Mod::EdgeAA::StaticApplyHooks()
{
	LoadTuning();

	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detourPrepareRender((char*)0x527E20, (char*)&HOOK_ScreenEdgeAA_PrepareRender, &ScreenEdgeAA_PrepareRender_Old, dis);
	detourPrepareRender.hook();
}

void Mod::EdgeAA::OnEndScene()
{
	if (Tuning.bEnable)
	{
		// The chain never reaches our PrepareRender hook while this is set,
		// so clear it every frame - the options screen can re-apply it at any
		// time (and it starts set unless the video option is already on).
		EdgeAADisabled = 0;
	}
}
