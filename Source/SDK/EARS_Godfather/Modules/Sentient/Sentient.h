#pragma once

#include "SDK/EARS_Godfather/Modules/PartedAnimated/PartedAnimated.h"

// SDK
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"

namespace EARS
{
	namespace Modules
	{
		class Family;
		class StandardDamageComponent;

		/**
		 * An object in the world to describe a sentient being.
		 */
		class Sentient : public PartedAnimated
		{
		public:

			// Getters
			EARS::Havok::CharacterProxy* GetCharacterProxy() { return &m_CharacterProxy; }
			EARS::Havok::CharacterProxy& GetCharacterProxyChecked() { return m_CharacterProxy; }
			EARS::Modules::Family* GetFamily() const { return m_Family.GetPtr(); }
			EARS::Modules::StandardDamageComponent* GetDamageComponent() const { return m_Damage; }
			EARS::Vehicles::WhiteboxCar* GetVehicle() const { return m_Vehicle.GetPtr(); }

		protected:

			char m_Padding_Sentient_1[0x30];
			EARS::Havok::CharacterProxy m_CharacterProxy;		// 0x3C0 - 0x4AC // (EARS::Modules::Sentient)
			char m_Padding_Sentient_2[0x148];
			EARS::Modules::StandardDamageComponent* m_Damage = nullptr; // 0x5F4 - 0x5F8 (EARS::Modules::Sentient)
			char m_Padding_Sentient_3[0x1848];
			SafePtr<EARS::Vehicles::WhiteboxCar> m_Vehicle;					// 0x1E40 - 0x1E48
			char m_Padding_Sentient_4[0x90];
			SafePtr<EARS::Modules::Family> m_Family;						// 0x1ED8 - 0x1EE0
		};
	}
}
