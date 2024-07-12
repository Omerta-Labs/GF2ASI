#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"

namespace EARS
{
	namespace Modules
	{
		class Family;

		/**
		 * A manager for Family objects in Godfather II.
		 */
		class FamilyManager
		{
		public:

			// TEMP -> Debug only
			void Test();

			// Fetch the instance of the Family Manager
			static FamilyManager* GetInstance();

		private:

			char m_Padding_FamilyManager_0[0x24];
			Array<EARS::Modules::Family*> m_Families;
			Array<EARS::Modules::Family*> m_StrategyFamilies;
			bool bHasStrategyGameStarted = false;
			bool bLetStrategyGameRun = false;

		};
	} // Modules
} // EARS
