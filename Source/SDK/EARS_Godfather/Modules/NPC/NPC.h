#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"

// SDK
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// CPP
#include <cstdint>

namespace EARS
{
	namespace Modules
	{
		class NPCCrewComponent;
		class NPCUpgradeComponent;

		/**
		 * The NPC class for The Godfather II
		 */
		class NPC : public Sentient
		{
		public:

			// Getters
			EARS::Modules::NPCCrewComponent* GetCrewComponent() const;
			EARS::Modules::NPCUpgradeComponent* GetUpgradeComponent() const;
			EARS::Modules::SimNPC* GetOwningSimNPC() const { return m_OwnerSimNPC.GetPtr(); }

		private:

			char m_Padding_NPC_0[0x2C];
			char m_DebugName[64];
			float m_LastMovementBlockedTime = 0.0f;
			void* m_PressureComponent = nullptr; // EARS::Modules::PressureComponent
			Flags32 m_RWSFlags;
			Flags32 m_Flags1;
			Flags32 m_Flags2;
			Flags32 m_RaycastLockFlags;
			char m_Padding_NPC_1[0x1BC];
			void* m_CrewSpecialtyIndicator = nullptr;
			char m_Padding_NPC_2[0x54];
			SafePtr<EARS::Modules::SimNPC> m_OwnerSimNPC;

		};
	} // Modules
} // EARS
