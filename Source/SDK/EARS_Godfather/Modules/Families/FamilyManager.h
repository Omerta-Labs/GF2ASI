#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"

// CPP
#include <functional>
#include <stdint.h>

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

			const EARS::Modules::Family* GetFamily(const uint32_t FamilyID) const;

			/**
			 * Utility function to iterate through all loaded Families
			 * All headers pass-by-ref, so none of them should be nullptr.
			 * Function does not exist in GF2 exe, merely utility header for us to use.
			 */
			typedef std::function<void(Family&)> TVisitFamilyFunctor;
			void ForEachFamily(const TVisitFamilyFunctor& InFunction);
			void ForEachStrategyFamily(const TVisitFamilyFunctor& InFunction);

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
