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

			void ActivateHUDIndicator(uint32_t NewIndicatorType, const char* InIndicatorVfxName);

			void DisableHUDIndicator();

			uint32_t GetHudIndicatorType() const { return m_HudIndicatorType; }
			const String& GetHudIndicatorVFXName() const { return m_HudIndicatorVFXName; }

			// Getters
			EARS::Modules::NPCCrewComponent* GetCrewComponent() const;
			EARS::Modules::NPCUpgradeComponent* GetUpgradeComponent() const;
			EARS::Modules::SimNPC* GetOwningSimNPC() const { return m_OwnerSimNPC.GetPtr(); }

		private:

			char m_Padding_NPC_1[0x1D4];
			uint32_t m_HudIndicatorType = 0;									// 0x2104	
			String m_HudIndicatorVFXName;										// 0x2108	
			char m_Padding_NPC_2[8];
			void* m_CrewSpecialtyIndicator = nullptr;							// 0x2120
			char m_Padding_NPC_3[0x54];
			SafePtr<EARS::Modules::SimNPC> m_OwnerSimNPC;						// 0x2178

		};

		static_assert(sizeof(NPC) == 8576);
	} // Modules
} // EARS
