#pragma once

namespace Mod
{
	/**
	 * Raises the resolution of the mobface head shot - the portrait of the
	 * player that the family tree, the pause map and the apparel screens show.
	 * Stock it is captured into a 128x128 texture, which is why it looks like a
	 * block of pixels next to the rest of the UI.
	 *
	 * The capture runs across three stages:
	 *
	 *  - MobFaceManager::RenderHeadShot, on the sim side, points the
	 *    "HeadShotCam" MobfaceCameraInfo at the player's m_mandible bone, locks
	 *    the model to the mobface pose and pushes near/far plus FoV onto the
	 *    "MobfaceRenderContext", then hands off to UIMAModelSubscriber, which
	 *    queues a DirectRenderModel command with isHeadShot set and the screen
	 *    dimensions taken from the front buffer - so the model is drawn at the
	 *    full display resolution, not into a small viewport.
	 *
	 *  - Pres_DirectRenderModel (0x953E40, render side) draws the model over
	 *    the whole render target, resolves it, then switches the render target
	 *    to the head shot texture and blits the frame back down with a single
	 *    quad. The horizontal UVs are 0.5 +/- (height / width) / 2, so what
	 *    lands in the texture is the centre square of the frame. At 1080p that
	 *    is a 1080x1080 crop squeezed into 128x128.
	 *
	 *  - AC_Delayed_DirectRenderTexture_remote_impl (0x938C40, from
	 *    ui_hstexturesubscriber.cpp) draws that texture into the UI widget
	 *    rect, sampling u 0.1 to 0.9 (0xE56790) to reframe the square as a
	 *    portrait. Only about 102 x 128 texels are ever visible.
	 *
	 * The texture itself is allocated once, in MobFaceManager::LoadResource
	 * (0x9BD020), by a TX_AllocTexture call whose width and height are both
	 * baked in as push 128. Nothing else reads the texture - the only users of
	 * m_headShotTexture are the two functions above - and it is allocated with
	 * a single mip level, so there is no mip chain or pitch assumption to break.
	 * Displ_InitFrameBuffers drives the very same allocator with the user's
	 * chosen display resolution, so arbitrary sizes are already well supported.
	 *
	 * That makes the fix a two operand patch: overwrite both immediates before
	 * the mobface resource loads and every later capture, including the ones
	 * after an unload/reload, uses the larger target.
	 *
	 * Note the downscale is one bilinear quad blit with no mip chain, so it is
	 * effectively a two by two tap of a much larger source. Raising the target
	 * recovers a great deal of detail, but very high values sharpen the source
	 * aliasing rather than remove it; 512 is a good balance and 1024 is about
	 * the point of diminishing returns.
	 *
	 * Controlled by [MugShot] Enable and Resolution in gf2asi.ini.
	 */
	class MugShotFix
	{
	public:

		// Rewrites the head shot texture dimensions in place. Must run before
		// the mobface resource is loaded, which is the only point the texture
		// is ever allocated.
		static void StaticApplyHooks();
	};
}
