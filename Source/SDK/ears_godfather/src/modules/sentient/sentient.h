#pragma once

#include "SDK/ears_godfather/src/modules/partedanimated/partedanimated.h"

// SDK
#include "SDK/ears_common/include/ears_common/safeptr.h"
#include "SDK/ears_godfather/src/modules/generic/scriptactor.h"
#include "SDK/ears_godfather/src/modules/npc/scriptedsequence.h"
#include "SDK/ears_godfather/src/modules/utils/WeaponUtils.h"
#include "SDK/ears_physics/src/ears_physics/characters/characterproxy.h"
#include "SDK/ears_statemachine/include/ears_statemachine/statemachine.h"

namespace EARS
{
	namespace Modules
	{
		class Family;
		class StandardDamageComponent;

		/**
		 * An object in the world to describe a sentient being.
		 * TODO: Find where EARS::Modules::SentientScriptActor lives in the hierarchy
		 */
		class Sentient :
			public PartedAnimated,
			public IScriptedSequenceTarget,
			public EARS::WeaponUtils::IDamageListener,
			public EARS::WeaponUtils::IHealthListener
		{
		public:

			virtual ~Sentient() = 0;

			// Head Tracking
			void DeactivateHeadTracking(bool bForceDeactivate);

			// Getters
			EARS::Havok::CharacterProxy* GetCharacterProxy() { return &m_CharacterProxy; }
			EARS::Havok::CharacterProxy& GetCharacterProxyChecked() { return m_CharacterProxy; }
			EARS::Modules::Family* GetFamily() const { return m_Family.GetPtr(); }
			EARS::Modules::StandardDamageComponent* GetDamageComponent() const { return m_Damage; }
			EARS::Vehicles::WhiteboxCar* GetVehicle() const { return m_Vehicle.GetPtr(); }
			EARS::StateMachineSys::StateMachine* GetRootStateMachine() { return m_SmTree.GetRoot(); }

			/** True if this Sentient is currently inside a vehicle.
			 *  PC ASM: [ecx+0x1B94], shr 0x13, and 1 — checks bit 19 of m_SentientStateFlags. */
			bool IsInVehicle() const { return (m_SentientStateFlags >> 19) & 1; }

		protected:

			// 0x4B0 is m_GraphClient
			// 0x5F0 is m_PathingType

			char m_Padding_Sentient_1[0x4];
			EARS::StateMachineSys::StateMachineTree m_SmTree;
			char m_Padding_Sentient_2[0xC];
			EARS::Havok::CharacterProxy m_CharacterProxy;		// 0x3C0 - 0x4AC // (EARS::Modules::Sentient)
			char m_Padding_Sentient_3[0x148];
			EARS::Modules::StandardDamageComponent* m_Damage = nullptr; // 0x5F4 - 0x5F8 (EARS::Modules::Sentient)
			char m_Padding_Sentient_4a[0x159C];
			uint32_t m_SentientStateFlags = 0;					// 0x1B94 — bit 19 (0x80000) = IsInVehicle
			char m_Padding_Sentient_4b[0x2A8];
			SafePtr<EARS::Vehicles::WhiteboxCar> m_Vehicle;					// 0x1E40 - 0x1E48
			char m_Padding_Sentient_5[0x90];
			SafePtr<EARS::Modules::Family> m_Family;						// 0x1ED8 - 0x1EE0
			char m_Padding_Sentient_6[0x50];

		};

		static_assert(sizeof(Sentient) == 0x1F30, "EARS::Framework::Sentient must equal 0x1F30");
	}
}
