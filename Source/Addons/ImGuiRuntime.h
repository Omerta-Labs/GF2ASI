#pragma once

//=============================================================================
// The ImGui context, its D3D9 and Win32 backends, and the frame loop.
//
// Knows nothing about what is drawn. Callers register panels, which run inside
// the frame between NewFrame and Render.
//
// Three threads touch this and the split matters:
//
//   SIM           Tick(), once per game tick, builds the frame
//   PRESENTATION  Present(), OnDeviceLost(), OnDeviceRestored()
//   window        WndProc()
//
// A single recursive mutex serialises all context access, so Present() renders
// a completed snapshot of the last frame rather than the live context. Without
// that, a present landing mid-build draws a half-built frame, and the number of
// presents per tick is not ours to control.
//=============================================================================

#include <windows.h>

namespace Mod::ImGuiRuntime
{
	/**
	 * Drawn inside the frame. Called on the SIM thread with the context lock
	 * held, so ImGui calls are safe and re-entering this namespace is too.
	 */
	using PanelFn = void (*)();

	/**
	 * Creates the context, initialises both backends against the game's window
	 * and device, applies the style and loads the default font. Returns false
	 * if the window or device handle is not up yet.
	 */
	bool Open();

	void Close();

	/** Returns false once the registry is full. Call during start-up. */
	bool RegisterPanel(PanelFn Panel);

	/**
	 * Builds one frame: new frame, every registered panel in registration
	 * order, then render and snapshot. SIM thread.
	 */
	void Tick();

	/** Draws the last completed frame. PRESENTATION thread. */
	void Present();

	void OnDeviceLost();
	void OnDeviceRestored();

	/**
	 * Discards the last completed frame. Ticks stop across a level change but
	 * presents keep coming, so without this the final menu frame lingers over
	 * the loading screen.
	 */
	void DropFrame();

	/** Only from a WndProc handler. Non-zero means the message was consumed. */
	LRESULT WndProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);

	/**
	 * Whether ImGui should take the mouse and keyboard. While owned it draws a
	 * software cursor and accepts input; while not, its input is masked off so
	 * it does not hover and click against a cursor the game is recentring.
	 */
	void SetInputOwned(bool bOwned);
	bool IsInputOwned();

	/**
	 * Records where the game last tried to warp the cursor while its
	 * recentring was being suppressed, so RestoreCursorPos can put it back.
	 * Called from the SetCursorPos detour, on the SIM thread.
	 */
	void NotifySuppressedCursorPos(int X, int Y);

	/**
	 * Puts the pointer back where the game last wanted it. The game measures
	 * mouse movement as an offset from where it last parked the pointer, so
	 * handing input back without this makes the first frame of camera input a
	 * full-screen jump.
	 */
	void RestoreCursorPos();

	/** Replaces the current font. Path is relative to the game directory. */
	bool AddFont(const char* Path);
}
