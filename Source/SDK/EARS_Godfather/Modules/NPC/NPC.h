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

			char m_Padding_NPC_1[0x1F0];
			void* m_CrewSpecialtyIndicator = nullptr;					// 0x2120
			char m_Padding_NPC_2[0x54];
			SafePtr<EARS::Modules::SimNPC> m_OwnerSimNPC;				// 0x2178

		};
	} // Modules
} // EARS
