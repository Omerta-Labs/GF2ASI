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
		void Player::TrySwapPlayerModel()
		{
			// FREDO
			m_PartsAssemblyName.m_pCStr = "unq_fredo_corleone";
			m_PartsAssemblyName.m_len = 20;
			m_PartsAssemblyName.m_allocLen = 32;

			MemUtils::CallClassMethod<void, EARS::Modules::Player*, void*, const char*>(0x09C58C0, this, nullptr, "unq_0005_miami");
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
