#include "Player.h"

// Framework
#include "SDK/EARS_Common/BitVector.h"
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

		bool Player::TestPlayerFlag(PlayerFlag InFlag) const
		{
			return BitVector_Test(m_PlayerFlags, (uint32_t)InFlag);
		}

		void Player::SetPlayerFlag(PlayerFlag InFlag)
		{
			BitVector_Set(m_PlayerFlags, (uint32_t)InFlag);
		}

		void Player::ClearPlayerFlag(EARS::Modules::PlayerFlag InFlag)
		{
			BitVector_Clear(m_PlayerFlags, (uint32_t)InFlag);
		}

		void Player::Teleport(const RwMatrixTag& InMatrix, uint32_t InOptions, RWS::CEventId* InEventMsg, StaticArray<void*, 4>* InPartitionList)
		{
			MemUtils::CallClassMethod<void, Player*, const RwMatrixTag&, uint32_t, RWS::CEventId*, StaticArray<void*, 4>*>(0x0079E3C0, this, InMatrix, InOptions, InEventMsg, InPartitionList);
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
