#pragma once

// GF2
#include "SDK/EARS_Common/Array.h"

// CPP
#include <functional>

namespace EARS
{
	namespace Modules
	{
		// forward decs
		class DemographicRegion;

		/**
		 * Manages Demographic Regions, objects which 
		 * manage population in game worlds
		 */
		class DemographicRegionManager
		{
		public:

			/**
			 * Fetch the current Demographic Region the Player is inside
			 * Updated per tick, as the Player moves around a city.
			 */
			EARS::Modules::DemographicRegion* GetCurrentRegion() const { return m_CurrentRegion; }

			/**
			 * Utility function to iterate through all loaded assembly header
			 * All const, we do not expect to modify any of the assemblies during iteration.
			 * All headers pass-by-ref, so none of them should be nullptr.
			 * Function does not exist in GF2 exe, merely utility header for us to use.
			 */
			typedef std::function<void(const DemographicRegion&)> TVisitDemographicRegionFunctor;
			void ForEachDemographicRegion(const TVisitDemographicRegionFunctor& InFunction) const;

			// Access to the Demographic Region Manager singleton instance
			static DemographicRegionManager* GetInstance();

		private:

			void* VTable_0 = nullptr;
			char m_Padding0[0xC];
			Array<EARS::Modules::DemographicRegion*> m_RegisteredRegions;
			EARS::Modules::DemographicRegion* m_CurrentRegion = nullptr;
		};

	} // Modules
} // EARS
