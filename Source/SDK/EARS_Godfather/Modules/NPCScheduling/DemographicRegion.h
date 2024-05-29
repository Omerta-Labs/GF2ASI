#pragma once

// CPP
#include <stdint.h>

struct guid128_t_2
{
	uint32_t a = 0;
	uint32_t b = 0;
	uint32_t c = 0;
	uint32_t d = 0;
};

struct SafePtrData
{
	const void* m_pObj; // SafeObj
	void* m_pNext; // SafePtrBase
};


namespace EARS
{
	namespace Modules
	{
		class DemographicRegion
		{
		public:

			void ApplyUserSettings();

			static void StaticApplyHooks();

		private:

			void* m_VTABLE = nullptr;

			struct sNPCFilterListData
			{
				guid128_t_2 m_ListGUID;
				SafePtrData m_ListEntity; // SafePtr<EARS::Modules::WeightedFilterList> 
				int32_t m_MinCount = 0;
				int32_t m_MaxCount = 0;
				float m_MinSpacing = 0.0f;
			};

			struct sVehicleListData
			{
				guid128_t_2 m_ListGUID;
				SafePtrData m_ListEntity; // SafePtr<EARS::Modules::WhiteboxCarList>
				int32_t m_MinCivilianCount = 0;
				int32_t m_MaxCivilianCount = 0;
				int32_t m_MinParkedCount = 0;
				int32_t m_MaxParkedCount = 0;
				uint32_t m_Flags = 0;
				float m_DamageChance = 0.0f;
				float m_DirtMin = 0.0f;
				float m_DirtMax = 0.0f;
			};

			char m_Padding0[0x58];
			sNPCFilterListData m_NPCFilters[3];
			sVehicleListData m_VehicleFilters[3];
		};
	} // Modules
} // EARS
