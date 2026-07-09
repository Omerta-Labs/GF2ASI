#pragma once

namespace SH
{
	struct ImGuiUISystem
	{
	public:

		void DrawTab();

		/**
		 * Update system when level services close.
		 * Drops our HUD state so the next level starts from the game's defaults.
		 */
		void CloseLevelServices();

	private:

		// Re-activate the HUD indicator on every currently spawned NPC
		void ShowAllNPCIndicators();

		// Disable the HUD indicator on every currently spawned NPC
		void HideAllNPCIndicators();

		bool bWantsUISuppressed = false;

		// Whether we've hidden the indicator markers above NPC heads.
		// Only applies to NPCs spawned at the time of the toggle; NPCs that
		// spawn afterwards will show theirs again (blocking that is TODO).
		bool bNPCIndicatorsHidden = false;
	};
}
