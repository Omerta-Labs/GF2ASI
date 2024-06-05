#pragma once

namespace EARS
{
	namespace Modules { class VehicleDamageComponent; }

	namespace Vehicles
	{
		class WhiteboxCar
		{
		public:

			// Getters
			EARS::Modules::VehicleDamageComponent* GetDamageComponent() const { return m_DamageComponent; }

		private:

			void* VTABLE = nullptr;
			char m_Padding0[0x7BC];
			EARS::Modules::VehicleDamageComponent* m_DamageComponent = nullptr;
		};
	} // Vehicles
} // EARS
