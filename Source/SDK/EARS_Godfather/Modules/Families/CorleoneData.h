#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Guid.h"

// CPP
#include <functional>
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * Family data, primarily stores information between SP and MP.
		 * Includes Honour data and Weapon Licenses for specific NPCs.
		 */
		class CorleoneFamilyData
		{
		public:

			static const uint8_t MIN_WEAPON_LICENSE = 1;
			static const uint8_t MAX_WEAPON_LICENSE = 4;

			struct HonorData
			{
			public:

				HonorData();
				HonorData(const HonorData& OtherData);
				HonorData(const EARS::Common::guid128_t& InSimNPC, const uint8_t InWeaponLicenseLevel);

				uint16_t m_HonorTotals[12];
				EARS::Common::guid128_t m_SimNpcGuid;
				uint8_t m_WeaponLicenseLevel = 0;
				uint8_t m_Unused = 0;
			};

			/**
			 * Fetch the Weapon License of a specific SimNPC.
			 * Use the GUID to query the weapon license.
			 * If the SimNPC is not find, then we assume they have MIN_WEAPON_LICENSE
			 */
			uint8_t GetWeaponLicense(const EARS::Common::guid128_t& SimNpcGuid) const;

			/**
			 * Set a specific Weapon License for a specific SimNPC.
			 * Must be within MIN_WEAPON_LICENSE and MAX_WEAPON_LICENSE
			 * TODO: Ensure that new SimNPCs are added to the data
			 */
			void SetWeaponLicense(const EARS::Common::guid128_t& SimNPCGuid, const uint8_t NewWeaponLevel);

			/**
			 * Utility function to iterate through all loaded Cities
			 * All const, we do not expect to modify any of the assemblies during iteration.
			 * All headers pass-by-ref, so none of them should be nullptr.
			 * Function does not exist in GF2 exe, merely utility header for us to use.
			 */
			typedef std::function<void(CorleoneFamilyData::HonorData&)> TVisitHonorDataFunctor;
			void ForEachHonourData(const TVisitHonorDataFunctor& InFunction);

			/** Access the CorleoneFamilyData instance */
			static CorleoneFamilyData* GetInstance();

		private:

			// Search for Honour Data associated with a specific SimNPC.
			// Return value is an index within the array.
			// Use the public API to fetch correct data
			int32_t FindHonourData(const EARS::Common::guid128_t& SimNpcGuid) const;

			void* vtbl;
			void* vtbl2;
			Array<EARS::Modules::CorleoneFamilyData::HonorData>  m_Honors;
			float m_Balance = 0.0f;
			bool m_PreOrderCrewUnlocked = false;
		};
	} // Framework
} // EARS
