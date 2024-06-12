#pragma once

#include "SDK/EARS_Godfather/Modules/PartedAnimated/PartedAnimated.h"

// SDK
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"

namespace EARS
{
	namespace Modules
	{
		class StandardDamageComponent;

		/**
		 * An object in the world to describe a sentient being.
		 */
		class Sentient : public PartedAnimated
		{
		public:

			void Translate(const float X, const float Y, const float Z);

			// Getters
			EARS::Havok::CharacterProxy* GetCharacterProxy() { return &m_CharacterProxy; }
			EARS::Modules::StandardDamageComponent* GetDamageComponent() const { return m_Damage; }
			EARS::Vehicles::WhiteboxCar* GetVehicle() const { return m_Vehicle.GetPtr(); }

		protected:

			char m_Padding_1[0xCC];
			EARS::Havok::CharacterProxy m_CharacterProxy;		// 0x3C0 - 0x4AC // (EARS::Modules::Sentient)
			char m_Padding_2[0x148];
			EARS::Modules::StandardDamageComponent* m_Damage = nullptr; // 0x5F4 - 0x5F8 (EARS::Modules::Sentient)
			char m_Padding_3[0x1848];
			SafePtr<EARS::Vehicles::WhiteboxCar> m_Vehicle;					// 0x1E40 - 0x1E48
		};
	}
}
