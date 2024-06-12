#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"

// SDK
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

// CPP
#include <cstdint>

namespace EARS
{
	namespace Modules
	{
		class PlayerUpgradeComponent;

		/**
		 * The Player class for The Godfather II
		 */
		class Player : public Sentient
		{
		public:

			// Getters
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
			char m_Padding_5[0x460];
			EARS::Common::guid128_t m_ModelGUID;
			uint32_t m_ModelStreamHandle = 0;
			EARS::Common::guid128_t m_PlayerRCB_GUID;
			EARS::Common::guid128_t m_FatherModelGUID;
			EARS::Common::guid128_t m_FatherRCB_GUID;
			int32_t m_PlayerDisableCount = 0;

		};
	} // Modules
} // EARS
