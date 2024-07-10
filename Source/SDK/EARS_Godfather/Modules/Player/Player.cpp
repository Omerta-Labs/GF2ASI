#include "Player.h"

// Addons
#include "Addons/tConsole.h"

// Framework
#include "SDK/EARS_Godfather/Framework/Core/Player/BasePlayer.h"
#include "SDK/EARS_Godfather/Framework/Core/Player/PlayerManager.h"
#include "SDK/EARS_Godfather/Modules/Components/PlayerUpgradeComponent.h"

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

/**
 * enum EARS::Modules::<unnamed-tag>
{
	kEvent_Run=0,
	kEvent_Crouch=1,
	kEvent_Vault=2,
	kEvent_QuickAttackA=3,
	kEvent_QuickAttackB=4,
	kEvent_LungeAttackLeft=5,
	kEvent_LungeAttackRight=6,
	kEvent_Block=7,
	kEvent_BlockIdle=8,
	kEvent_BlockDodgeLeft=9,
	kEvent_BlockDodgeRight=10,
	kEvent_BlockDodgeBack=11,
	kEvent_Grab=12,
	kEvent_GrabAlternate=13,
	kEvent_GrabIdle=14,
	kEvent_GrabRelease=15,
	kEvent_GrabQuickAttackA=16,
	kEvent_GrabQuickAttackB=17,
	kEvent_GrabQuickAttackC=18,
	kEvent_GrabChargeAttackStart=19,
	kEvent_GrabChargeAttackHold=20,
	kEvent_GrabChargeAttackFinish=21,
	kEvent_GrabPullup=22,
	kEvent_GrabPullup2=23,
	kEvent_GrabMoveForwardsRight=24,
	kEvent_GrabMoveBackwardsRight=25,
	kEvent_GrabMoveBackwardsLeft=26,
	kEvent_GrabMoveForwardsLeft=27,
	kEvent_GrabThrowLeft=28,
	kEvent_GrabThrowRight=29,
	kEvent_GrabStrangleA=30,
	kEvent_GrabStrangleB=31,
	kEvent_Garotte_Grab=32,
	kEvent_Garotte_Idle=33,
	kEvent_Garotte_Pullup=34,
	kEvent_ExecuteAttack_Engage=35,
	kEvent_SlamForward_Engage=36,
	kEvent_SlamLeft_Engage=37,
	kEvent_SlamRight_Engage=38,
	kEvent_HoldThrow_Engage=39,
	kEvent_HoldThrow_Disengage=40,
	kEvent_HoldThrow_Hold=41,
	kEvent_HoldThrow_Throw=42,
	kEvent_F2FActivate=43,
	kEvent_F2FUp=44,
	kEvent_F2FDecisionDown=45,
	kEvent_F2fDecisionLeft=46,
	kEvent_F2FDecisionRight=47,
	kEvent_NextF2FDebug=48,
	kEvent_PrevF2FDebug=49,
	kEvent_F2FDeactivate=50,
	kEvent_Activate=51,
	kEvent_ActivateHoldDown=52,
	kEvent_Deactivate=53,
	kEvent_DoorCSAActivate=54,
	kEvent_UseItem=55,
	kEvent_RevealItem=56,
	kEvent_ConcealItem=57,
	kEvent_NextItem=58,
	kEvent_PreviousItem=59,
	kEvent_TargetLock=60,
	kEvent_CameraReset=61,
	kEvent_CommitSuicide=62,
	kEvent_Revive=63,
	kEvent_WallWalk_Engage=64,
	kEvent_NeckSnap=65,
	kEvent_CrewAction=66,
	kEvent_CrewReturn=67,
	kEvent_CrewForceAction=68,
	kEvent_CrewForceReturn=69,
	kEvent_FlickTargetNext=70,
	kEvent_FlickTargetLeft=71,
	kEvent_FlickTargetRight=72,
	kEvent_NumEvents=73
};
 */