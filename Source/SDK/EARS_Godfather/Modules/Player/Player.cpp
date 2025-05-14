#include "Player.h"

// Addons
#include "Addons/tConsole.h"

// Framework
#include "SDK/EARS_Framework/Core/Player/PlayerManager.h"
#include "SDK/EARS_Godfather/Modules/Components/PlayerUpgradeComponent.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerMasterSM.h"

// Hook
#include <Addons/Hook.h>

namespace EARS
{
	namespace Modules
	{

		EARS::Modules::PlayerUpgradeComponent* Player::GetUpgradeComponent() const
		{
			EARS::Framework::Component* FoundComp = MemUtils::CallClassMethod<EARS::Framework::Component*, const EARS::Modules::Player*, uint32_t>(0x043B870, this, EARS::Modules::PlayerUpgradeComponent::GetComponentIndex());
			return (EARS::Modules::PlayerUpgradeComponent*)FoundComp;
		}

		EARS::Modules::PlayerMasterSM* Player::GetPlayerMasterStateMachine()
		{
			if (EARS::StateMachineSys::StateMachine* TreeSM = GetRootStateMachine())
			{
				const uint32_t MachineID = TreeSM->GetStateMachineID();
				if (MachineID == 0xB08AE1F6)
				{
					return reinterpret_cast<EARS::Modules::PlayerMasterSM*>(TreeSM);
				}
			}

			return nullptr;
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
