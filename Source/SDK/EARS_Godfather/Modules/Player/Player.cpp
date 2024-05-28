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
		void Player::TrySwapPlayerModel(const char* AssemblyName, const char* PresetName)
		{
			// FREDO
			m_PartsAssemblyName.m_pCStr = AssemblyName;
			m_PartsAssemblyName.m_len = strlen(AssemblyName);
			m_PartsAssemblyName.m_allocLen = 32;

			MemUtils::CallClassMethod<void, EARS::Modules::Player*, void*, const char*>(0x09C58C0, this, nullptr, PresetName);
		}

		void Player::Translate(const float X, const float Y, const float Z)
		{
			struct RwV3d
			{
				float x, y, z;
			};

			MemUtils::CallClassMethod<void, EARS::Modules::Player*, const RwV3d&>(0x07FF500, this, RwV3d(X, Y, Z));
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
