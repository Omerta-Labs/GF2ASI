#pragma once

namespace Mod
{
	/**
	 * Forces the game's EdgeAA post-process on every frame and exposes its
	 * tuning parameters through gf2asi.ini.
	 *
	 * EdgeAA (EARS::Alchemy::ScreenEdgeAA) is a depth-based edge detect +
	 * directional blur pass, not hardware MSAA. The engine treats it as a
	 * screen effect rather than a permanent AA stage: it only renders on
	 * frames where level/camera data submits parameters (a TEffectPresScreen
	 * of type 0x10), and the parameter block is zeroed again after every
	 * consumed frame. In practice most scenes never submit anything, so edges
	 * stay unsmoothed even with the EdgeAA video option enabled.
	 *
	 * The fix detours ScreenEdgeAA::PrepareRender, which the post-FX chain
	 * calls once per frame after all screen-effect submissions and before it
	 * decides which passes run - the ideal spot to (re)write the parameter
	 * block so the pass always runs with our tuned values. The chain skips
	 * PrepareRender entirely while the renderer-side disable flag is set
	 * (QualityOptions.EdgeAA off), so OnEndScene re-arms that flag as well.
	 *
	 * Controlled by the [EdgeAA] section in gf2asi.ini (default on).
	 */
	class EdgeAA
	{
	public:

		// Install the ScreenEdgeAA::PrepareRender detour.
		static void StaticApplyHooks();

		// Keeps the renderer-side "EdgeAA disabled" flag cleared so the pass
		// is not skipped when the video option is off. Call inside the
		// Displ_EndScene hook.
		static void OnEndScene();
	};
}
