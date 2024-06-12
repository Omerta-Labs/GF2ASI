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
