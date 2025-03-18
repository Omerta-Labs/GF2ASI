#pragma once

// SDK
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Framework/Core/Entity/Entity.h"

namespace EARS
{
	namespace Modules { class VehicleDamageComponent; class PresetInstance; }

	namespace Vehicles
	{
		/**
		 * A Car in the world of Godfather II.
		 */
		class WhiteboxCar : public EARS::Framework::Entity
		{
		public:

			// Getters
			EARS::Modules::VehicleDamageComponent* GetDamageComponent() const { return m_DamageComponent; }

		private:

			char m_WhiteboxCar_Padding0[0x640];
			EARS::Modules::VehicleDamageComponent* m_DamageComponent = nullptr; // 0x7C0
			char m_WhiteboxCar_Padding1[0x698];
			String m_PartsPresetName;											// 0xE5C
			String m_AssemblyName;												// 0xE6C
			EARS::Modules::PresetInstance* m_PresetInstance = nullptr;			// 0xE7C
			char m_WhiteboxCar_Padding2[0x88];
			EARS::Common::guid128_t m_ModelStrGuid;
			char m_WhiteboxCar_Padding3[0x8];
		};

		static_assert(sizeof(WhiteboxCar) == 0xF20, "EARS::Vehicles::WhiteboxCar must equal 0xF20");
	} // Vehicles
} // EARS
