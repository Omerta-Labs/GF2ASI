#pragma once

// Common
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/SafePtr.h"

// Framework
#include "SDK/EARS_Framework/Core/Base/Base.h"

// CPP
#include <stdint.h>
#include <string>


namespace EARS
{
	namespace Modules
	{
		class DemographicRegion : EARS::Framework::Base
		{
		public:

			void ApplyUserSettings();

			/**
			 * Because the game removed ability to load from command ID 0,
			 * this function is used to fetch the name on demand at runtime.
			 * Note that this is likely to be costly, so use caution.
			 * @return std::string - If found, the debug name of the Demographic Region.
			 */
			std::string GetDebugName() const;

			static void StaticApplyHooks();

		private:

			struct sNPCFilterListData
			{
				EARS::Common::guid128_t m_ListGUID;
				SafePtr<void*> m_ListEntity; // SafePtr<EARS::Modules::WeightedFilterList> 
				int32_t m_MinCount = 0;
				int32_t m_MaxCount = 0;
				float m_MinSpacing = 0.0f;
			};

			struct sVehicleListData
			{
				EARS::Common::guid128_t m_ListGUID;
				SafePtr<void*> m_ListEntity; // SafePtr<EARS::Modules::WhiteboxCarList>
				int32_t m_MinCivilianCount = 0;
				int32_t m_MaxCivilianCount = 0;
				int32_t m_MinParkedCount = 0;
				int32_t m_MaxParkedCount = 0;
				uint32_t m_Flags = 0;
				float m_DamageChance = 0.0f;
				float m_DirtMin = 0.0f;
				float m_DirtMax = 0.0f;
			};

			Array<SafePtr<void>> m_Volumes; // DemographicVolume
			sNPCFilterListData m_NPCFilters[3];
			sVehicleListData m_VehicleFilters[3];
		};
	} // Modules
} // EARS
