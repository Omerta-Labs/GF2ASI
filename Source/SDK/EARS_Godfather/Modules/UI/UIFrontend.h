#pragma once

namespace EARS::Apt
{
	class UIFrontend
	{
	public:

		void EnterState_StartNewGame(int ePreviousState);

		/** get game instance of this object */
		static UIFrontend* GetInstance();

	private:

		// Used to trigger new game start
		// Installs DLC features, hides menu
		void StartNewGame();
	};
}
