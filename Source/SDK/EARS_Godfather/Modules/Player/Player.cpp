#include "Player.h"

// Addons
#include "Addons/tConsole.h"

// Framework
#include "SDK/EARS_Godfather/Framework/Core/Player/BasePlayer.h"
#include "SDK/EARS_Godfather/Framework/Core/Player/PlayerManager.h"

// Hook
#include <Addons/Hook.h>

namespace EARS
{
	namespace Modules
	{
		void Player::HandleEvents(RWS::CMsg& MsgEvent)
		{
			hook::Type<RWS::CEventId> PlayerFatherAttachEventId = hook::Type<RWS::CEventId>(0x112B41C);
			hook::Type<RWS::CEventId> PlayerFatherDetachEventId = hook::Type<RWS::CEventId>(0x112B544);

			// check for attach and detach
			if (MsgEvent.IsEvent(PlayerFatherAttachEventId))
			{
				tConsole::fWriteLine("Player wants to attach father model");
			}
			else if (MsgEvent.IsEvent(PlayerFatherDetachEventId))
			{
				tConsole::fWriteLine("Player wants to detach father model");
			}
		}

		void Player::TrySwapPlayerModel()
		{
			// FREDO
			m_FatherModelGUID.a = 3858313425;
			m_FatherModelGUID.b = 3858313425;
			m_FatherModelGUID.c = 3858313425;
			m_FatherModelGUID.d = 3858313425;
			m_ModelGUID.a = 3858313425;
			m_ModelGUID.b = 3858313425;
			m_ModelGUID.c = 3858313425;
			m_ModelGUID.d = 3858313425;

			//m_FatherModelGUID.a = m_ModelGUID.a;
			//m_FatherModelGUID.b = m_ModelGUID.b;
			//m_FatherModelGUID.c = m_ModelGUID.c;
			//m_FatherModelGUID.d = m_ModelGUID.d;
			m_FatherRCB_GUID.a = m_PlayerRCB_GUID.a;
			m_FatherRCB_GUID.b = m_PlayerRCB_GUID.b;
			m_FatherRCB_GUID.c = m_PlayerRCB_GUID.c;
			m_FatherRCB_GUID.d = m_PlayerRCB_GUID.d;

			//m_PartsPresetName.m_pCStr = "unq_0005_miami";
			//m_PartsPresetName.m_len = 16;
			//m_PartsPresetName.m_allocLen = 32;
			//m_PartsThemeName.m_pCStr = "unq_0005_main";
			//m_PartsThemeName.m_len = 15;
			//m_PartsThemeName.m_allocLen = 32;
			//m_PartsSwapSetName.m_pCStr = "a_default";
			//m_PartsSwapSetName.m_len = 11;
			//m_PartsSwapSetName.m_allocLen = 16;
			m_PartsAssemblyName.m_pCStr = "unq_fredo_corleone";
			m_PartsAssemblyName.m_len = 20;
			m_PartsAssemblyName.m_allocLen = 32;


			hook::Type<RWS::CEventId> PlayerFatherAttachEventId = hook::Type<RWS::CEventId>(0x112B41C);
			hook::Type<RWS::CEventId> PlayerFatherDetachEventId = hook::Type<RWS::CEventId>(0x112B544);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerFatherAttachEventId, false);

			//MemUtils::CallClassMethod<bool, Player*, int>(0x07A60C0, this, 2);
		}

		Player* Player::GetLocalPlayer()
		{
			// Use the PlayerManager in the games core framework to fetch the local player and cast to a Player
			if (EARS::Framework::PlayerManager* PlayerMgr = EARS::Framework::PlayerManager::GetInstance())
			{
				EARS::Framework::BasePlayer* LocalPlayer = PlayerMgr->GetPlayer(0);
				return (Player*)(LocalPlayer - 0x7CC);
			}
			return nullptr;
		}
	} // Modules
} // EARS
