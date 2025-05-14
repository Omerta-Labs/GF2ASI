#pragma once

// SDK Common
#include "SDK/EARS_Common/Guid.h"

// SDK Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/Player/BasePlayer.h"

// SDK Modules
#include "SDK/EARS_Godfather/Modules/Player/PlayerActionable.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerCamera.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDefines.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerF2FInfo.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerLogical.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerLowerBody.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerUpperBody.h"
#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"

// CPP
#include <cstdint>

namespace EARS
{
	namespace Modules
	{
		class InventoryManager;
		class PlayerMasterSM;
		class PlayerUpgradeComponent;

		/**
		 * The Player class for The Godfather II
		 */
		class Player : public Sentient/*, public EARS::Framework::BasePlayer*/
		{
		public:

			virtual ~Player() = 0;

			// Getters
			EARS::Modules::InventoryManager* GetInventoryManager() const { return m_InventoryManager; }
			EARS::Modules::PlayerUpgradeComponent* GetUpgradeComponent() const;
			EARS::Modules::PlayerMasterSM* GetPlayerMasterStateMachine();

			// TODO: These are getters for PlayerSM State Machine, however its not const.
			// I'd expect them to be const here, but as we continue to reverse engineer we may find that
			// the State Machines actively edit this data
			EARS::Modules::PlayerActionableInfo* GetActionableInfo() { return &m_ActionableInfo; }
			EARS::Modules::PlayerCameraInfo* GetCameraInfo() { return &m_CameraInfo; }
			EARS::Modules::PlayerF2FInfo* GetF2FInfo() { return &m_F2FInfo; }
			EARS::Modules::PlayerLogicalInfo* GetLogicalInfo() { return &m_LogicalInfo; }
			EARS::Modules::PlayerLowerBodyInfo* GetLowerBodyInfo() { return &m_LowerBodyInfo; }
			EARS::Modules::PlayerUpperBodyInfo* GetUpperBodyInfo() { return &m_UpperBodyInfo; }

			// TODO: This is part of EARS::Framework::BasePlayer but cannot get virtual table to work...
			uint32_t GetControllerID() const { return m_ControllerID; }

			// TODO: This is part of EARS::Framework::BasePlayer but cannot get virtual table to work...
			uint32_t GetPlayerID() const { return m_PlayerID; }

			/**
			 * Get the Local Player
			 * *NOT PART OF MAIN GAME, UTILITY FUNCTION*
			 * @return Player - The Local Player
			 */
			static Player* GetLocalPlayer();

		private:

			void* m_BasePlayerVTBL = nullptr;
			char m_Padding_4[0x55C];
			uint32_t m_CurrentModelType = 0;		// 0x2490 - 0x2494
			uint32_t m_PlayerID = 0;				// 0x2494 - 0x2498
			uint32_t m_ControllerID = 0;			// 0x2498 - 0x249C
			uint32_t m_PlayerFlags[3];				// 0x249C - 0x24A8
			EARS::Modules::InventoryManager* m_InventoryManager = nullptr;
			char m_Padding_5[0x1F4];
			EARS::Modules::PlayerUpperBodyInfo m_UpperBodyInfo;
			EARS::Modules::PlayerLowerBodyInfo m_LowerBodyInfo;
			EARS::Modules::PlayerF2FInfo m_F2FInfo;
			EARS::Modules::PlayerLogicalInfo m_LogicalInfo;
			EARS::Modules::PlayerCameraInfo m_CameraInfo;				// 0x2738
			EARS::Modules::PlayerActionableInfo m_ActionableInfo;		// 0x2744
			char m_Padding_6[0x80];
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
