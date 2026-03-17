#pragma once

// C++
#include <stdint.h>

namespace EARS::Apt
{
	class UIFrontend
	{
	public:

		enum class UIFrontendFlags : int32_t
		{
			FE_FLAG_ALLOW_STATE_CHANGES = 0x10,
		};

		enum class FrontendState : uint32_t
		{
			NONE = 0x0,
			MAIN_MENU = 0x11,
			START_NEW_GAME = 0x16
		};

		using StateFunc = void(__cdecl*)(EARS::Apt::UIFrontend*, EARS::Apt::UIFrontend::FrontendState);

		struct StateData
		{
			uint32_t m_Flags = 0;
			FrontendState m_State = FrontendState::NONE;
			StateFunc m_Func = nullptr;
		};

		void EnterState_StartNewGame(int ePreviousState);

		// Used to trigger new game start
		// Installs DLC features, hides menu
		void StartNewGame();

		void ExecuteNextState();

		void AddNextStateSequence(int DelayTimeMS);

		/** get game instance of this object */
		static UIFrontend* GetInstance();

		static void PatchState(int State, StateFunc Function);

		static void CopyStates();

	private:
	};
}
