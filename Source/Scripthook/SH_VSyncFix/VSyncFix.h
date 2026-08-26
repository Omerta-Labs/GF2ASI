#pragma once

namespace Mod
{
	/**
	 * Fixes the game locking to 30 FPS whenever the VSync option is enabled,
	 * regardless of the selected resolution and refresh rate.
	 *
	 * The renderer builds its present parameters in Displ_BuildPresentParams
	 * (0x6073E0) and enables VSync in two mutually exclusive ways:
	 *
	 *  - In fullscreen it asks the adapter caps whether D3DPRESENT_INTERVAL_TWO
	 *    is supported and, if so, always uses it. Every real D3D9 driver reports
	 *    the whole interval mask, so the test always passes and the game presents
	 *    on every second vblank - exactly half the refresh rate. The branch that
	 *    would have selected D3DPRESENT_INTERVAL_ONE is unreachable.
	 *
	 *  - In windowed mode the present interval is always D3DPRESENT_INTERVAL_
	 *    IMMEDIATE, so D3D never syncs at all. VSync is emulated instead:
	 *    Displ_SwapScreens calls Displ_WaitForFrameLock with a vblank count of
	 *    2, and that busy-waits for count * 16.666664 ms, i.e. 33.3 ms. The
	 *    period is hardcoded to a 60 Hz assumption, so the refresh setting is
	 *    ignored here as well.
	 *
	 * The two are kept apart by the flag at 0x120575A, which is really "use the
	 * software frame lock": it is cleared only when the hardware is already
	 * presenting on every second vblank. Both routes therefore land on 30 FPS.
	 *
	 * The fix has to cover both:
	 *
	 *  - The fullscreen interval is corrected in place, inside the functions
	 *    that decide it, so every caller and every later device reset picks it
	 *    up. Detouring them is not practical - they take their arguments in
	 *    EDX, ECX and ESI, which no MSVC calling convention can express. There
	 *    are two: Displ_BuildPresentParams and the near-identical
	 *    Displ_BuildFallbackPresentParams (0x608B50), which Displ_CreateDevice
	 *    falls back to and which carries a duplicate of the same bug.
	 *
	 *  - Displ_WaitForFrameLock is detoured. In fullscreen it now returns
	 *    immediately, because the corrected present interval makes the hardware
	 *    do the sync. In windowed mode it paces to one real refresh period
	 *    instead of two hardcoded 60 Hz ones.
	 *
	 * Controlled by [Fixes] ApplyVSyncFix in gf2asi_settings.ini (default on).
	 */
	class VSyncFix
	{
	public:

		// Corrects the fullscreen present interval and installs the frame lock
		// detour. Must run before the game creates its device, so the very
		// first CreateDevice already gets the corrected interval.
		static void StaticApplyHooks();
	};
}
