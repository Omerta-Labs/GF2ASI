#pragma once

namespace EARS::Modules
{
	class MarketingCameraInfo;
}

namespace SH
{
	struct ImGuiPhotoModeSystem
	{
	public:

		void DrawTab();

		/**
		 * Toggle photo mode on/off. Used by the keybind shortcut so it stays in sync
		 * with the mod menu's Enter/Exit buttons.
		 */
		void Toggle();

		/** Whether photo mode is currently active. */
		bool IsActive() const { return PhotoModeCameraInfo != nullptr; }

		/**
		 * Update system when level services close.
		 * Ensures photo mode is fully exited so we don't hold onto a stale camera.
		 */
		void CloseLevelServices();

	private:

		void StartPhotoMode();

		void StopPhotoMode();

		// Toggle the player's controls via the game's enable/disable control events
		void SetPlayerFrozen(bool bFreeze);

		// Toggle the gameplay pause, keeping the photo mode camera updating while paused
		void SetTimeFrozen(bool bFreeze);

		// Draw the post-process effect toggles (letterbox, depth of field, blur, bloom)
		void DrawEffectsSection(bool bInPhotoMode);

		// Ramp out any effects we enabled and reset their toggles
		void StopAllEffects();

		EARS::Modules::MarketingCameraInfo* PhotoModeCameraInfo = nullptr;

		// FOV (radians) of the game camera when photo mode was entered, used by "Reset"
		float DefaultCameraFOV = 0.0f;

		bool bPlayerFrozen = false;
		bool bTimeFrozen = false;

		// Post-process effect state; effects are only enabled while in photo mode
		bool bLetterboxEnabled = false;
		bool bDepthOfFieldEnabled = false;
		bool bBlurEnabled = false;
		bool bBloomEnabled = false;

		// Effect tuning, defaults matching the game's own (focus 10.0, intensity 1.0)
		float DepthOfFieldFocus = 10.0f;
		float DepthOfFieldIntensity = 1.0f;
		float BlurIntensity = 1.0f;
		float BloomIntensity = 1.0f;
	};
}
