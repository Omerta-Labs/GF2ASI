#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Entity/Entity.h"

namespace EARS
{
	namespace Modules { class VehicleDamageComponent; }

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

			char m_Padding0[0x650];
			EARS::Modules::VehicleDamageComponent* m_DamageComponent = nullptr; // 0x7C0
		};
	} // Vehicles
} // EARS
