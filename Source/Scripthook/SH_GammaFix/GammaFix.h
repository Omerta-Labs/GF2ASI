#pragma once

namespace Mod
{
	/**
	 * Fixes the game's brightness/gamma option behaving differently between
	 * fullscreen and windowed mode.
	 *
	 * The game applies its video-calibration gamma exclusively through the D3D9
	 * hardware gamma ramp (Displ_SetGamma -> IDirect3DDevice9::SetGammaRamp),
	 * which only takes effect in exclusive fullscreen mode:
	 *
	 *  - In windowed mode the call is a silent no-op, so the brightness slider
	 *    does nothing. We apply the same curve as a post-process pixel shader
	 *    at the end of each frame instead.
	 *
	 *  - In fullscreen mode setting a ramp replaces the desktop calibration
	 *    ramp (ICC profile, night light etc.), even when the game pushes its
	 *    default identity curve. We skip identity ramps and restore the
	 *    captured desktop ramp when the slider returns to default.
	 *
	 * Controlled by [Fixes] ApplyBrightnessFix in gf2asi.ini (default on).
	 *
	 * Separately, [Fixes] GammaContrast sets the first slot of the curve the
	 * game passes to Displ_SetGamma - the exponent the ramp is raised to.
	 * Range is [0.8, 1.2]; the PC build runs at 0.8, the console versions at
	 * 1.0. The in-game video calibration screen that would otherwise drive it
	 * is dead on PC, so this always replaces whatever the game asks for, with
	 * the brightness fix on or off.
	 */
	class GammaFix
	{
	public:

		// Install the Displ_SetGamma detour and capture the desktop gamma ramp.
		// Must run before the game creates its device and enters fullscreen.
		static void StaticApplyHooks();

		// Applies the gamma curve as a fullscreen pass in windowed mode.
		// Call inside the Displ_EndScene hook, before the real EndScene.
		static void OnEndScene();

		// Releases the D3DPOOL_DEFAULT objects so the device can reset.
		// Call inside the Displ_ResetDevice hook, before the real reset.
		static void OnDeviceReset();
	};
}
