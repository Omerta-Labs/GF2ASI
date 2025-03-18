#pragma once

// SDK Common
#include "SDK/EARS_Common/Guid.h"

// SDK Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// SDK Modules
#include "SDK/EARS_Godfather/Modules/Player/PlayerDefines.h"
#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"

// CPP
#include <cstdint>

namespace EARS
{
	namespace Modules
	{
		class InventoryManager;
		class PlayerUpgradeComponent;

		/**
		 * The Player class for The Godfather II
		 */
		class Player : public Sentient
		{
		public:

			// Getters
			EARS::Modules::InventoryManager* GetInventoryManager() const { return m_InventoryManager; }
			EARS::Modules::PlayerUpgradeComponent* GetUpgradeComponent() const;

			/**
			 * Get the Local Player
			 * *NOT PART OF MAIN GAME, UTILITY FUNCTION*
			 * @return Player - The Local Player
			 */
			static Player* GetLocalPlayer();

		private:

			char m_Padding_4[0x5B0];
			uint32_t m_CurrentModelType = 0;		// 0x2490 - 0x2494
			uint32_t m_PlayerID = 0;				// 0x2494 - 0x2498
			uint32_t m_ControllerID = 0;			// 0x2498 - 0x249C
			uint32_t m_PlayerFlags[3];				// 0x249C - 0x24A8
			EARS::Modules::InventoryManager* m_InventoryManager = nullptr;
			char m_Padding_5[0x45C];
			EARS::Common::guid128_t m_ModelGUID;
			uint32_t m_ModelStreamHandle = 0;
			EARS::Common::guid128_t m_PlayerRCB_GUID;
			EARS::Common::guid128_t m_FatherModelGUID;
			EARS::Common::guid128_t m_FatherRCB_GUID;
			int32_t m_PlayerDisableCount = 0;
			uint32_t m_EventHashNames[EARS::Modules::PlayerActionEvents::kEvent_NumEvents];
			uint32_t m_VibrationHashNames[EARS::Modules::PlayerVibrationEvents::kVib_NumEvents];
			char m_Player_Padding_5[0x6BC];
		};

		static_assert(sizeof(Player) == 0x31D0, "EARS::Modules::Player must equal 0x31D0");
	} // Modules
} // EARS
