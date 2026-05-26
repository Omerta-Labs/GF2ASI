#pragma once

// SDK
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Physics/Behaviours/Vehicles/BaseVehicle.h"

namespace EARS
{
	namespace Modules { class VehicleDamageComponent; class PresetInstance; }

	namespace Vehicles
	{
		// forward declares
		class HavokWheeledVehicle;

		/**
		 * A Car in the world of Godfather II.
		 */
		class WhiteboxCar : public EARS::Vehicles::BaseVehicle
		{
		public:

			// Getters
			EARS::Modules::VehicleDamageComponent* GetDamageComponent() const { return m_DamageComponent; }

		private:

			// BaseVehicle
			char m_BaseVehicle_Padding0[0x24];									// 0x2B0
			bool m_bBaseVehicleInitialised = false;								// 0x2D8

			// WheeledVehicle
			char m_WheeledVehicle_Padding0[0x8A];								// 0x2D9
			HavokWheeledVehicle* m_HavokWheeledVehicle = nullptr;				// 0x360
			char m_WheeledVehicle_Padding1[0x2C];
			int m_UsePhysics = 0;												// 0x390
			char m_WhiteboxCar_Padding0[0x42C];

			// WhiteboxCar
			EARS::Modules::VehicleDamageComponent* m_DamageComponent = nullptr; // 0x7C0
			char m_WhiteboxCar_Padding1[0x698];
			String m_PartsPresetName;											// 0xE5C
			String m_AssemblyName;												// 0xE6C
			EARS::Modules::PresetInstance* m_PresetInstance = nullptr;			// 0xE7C
			char m_WhiteboxCar_Padding2[0x88];
			EARS::Common::guid128_t m_ModelStrGuid;
			char m_WhiteboxCar_Padding3[0x8];

			// NB:
			//0x2D8 -> m_bBaseVehicleInitialised;
			//0x360 -> m_HavokWheeledVehicle
			//0x390 -> m_bUsePhysics
		};

		static_assert(sizeof(WhiteboxCar) == 0xF20, "EARS::Vehicles::WhiteboxCar must equal 0xF20");
	} // Vehicles
} // EARS
