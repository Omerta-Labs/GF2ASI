#include "PlayerMasterSM_Modded.h"

#include "Addons/Settings.h"

// Scripthook
#include "Scripthook/SH_PlayerMasterSM/PlayerDebugOptions_Modded.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

#include <Windows.h>

namespace SH
{
	PlayerMasterSM_Modded::PlayerMasterSM_Modded(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
		: EARS::Modules::PlayerMasterSM(id, pSMParams)
		, CurrentState(EActiveState::Game)
	{
	}

	PlayerMasterSM_Modded::~PlayerMasterSM_Modded()
	{
	}

	uint32_t PlayerMasterSM_Modded::GetStateMachineID() const
	{
		// We are still considered PlayerMasterSM
		return 0xB08AE1F6;
	}

	bool PlayerMasterSM_Modded::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		if (MessageID == 0x900)
		{
			UpdateFlyMode();
			return true;
		}

		return EARS::Modules::PlayerMasterSM::HandleStateMessage(SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerMasterSM_Modded::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		if (TransID == 0x11)
		{
			const SH::PlayerDebugOptions_Modded& DebugOptions = *SH::PlayerDebugOptions_Modded::GetInstance();
			return DebugOptions.IsInAnimViewMode();
		}

		return EARS::Modules::PlayerMasterSM::CheckTransition(SimTime, FrameTime, TransID, TransData);
	}

	void PlayerMasterSM_Modded::InitialiseChild(StateMachine* ChildMachine)
	{
		EARS::Modules::PlayerMasterSM::InitialiseChild(ChildMachine);
	}

	void PlayerMasterSM_Modded::UpdateFlyMode()
	{
		const EARS::Modules::PlayerDebugOptions& DebugOptions = *EARS::Modules::PlayerDebugOptions::GetInstance();
		const bool bWantsFlyMode = DebugOptions.IsInDebugFly();
		bool bIsFlying = (CurrentState == EActiveState::FlyMode);

		// check if the state is mismatched
		if (bIsFlying != bWantsFlyMode)
		{
			if (EARS::Modules::Player* CurPlayer = GetPlayer())
			{
				EARS::Havok::CharacterProxy& CharProxy = CurPlayer->GetCharacterProxyChecked();
				const EARS::Havok::CharacterProxy::CollisionState NewState = (bWantsFlyMode ? EARS::Havok::CharacterProxy::CollisionState::CS_TRIGGERS_ONLY : EARS::Havok::CharacterProxy::CollisionState::CS_ENABLED);
				CharProxy.SetCollisionState(NewState);
			}

			CurrentState = (bWantsFlyMode ? EActiveState::FlyMode : EActiveState::Game);
		}

		// update flag
		bIsFlying = (CurrentState == EActiveState::FlyMode);

		// now listen for players inputs
		if (bIsFlying)
		{
			if (EARS::Modules::Player* CurPlayer = GetPlayer())
			{
				Settings& LclSettings = Settings::GetCheckedRef();

				if (GetAsyncKeyState(LclSettings.GetFlyModeUpInput()) & 0x8001)
				{
					CurPlayer->Translate(RwV3d(0.0f, 0.1f, 0.0f));
				}

				if (GetAsyncKeyState(LclSettings.GetFlyModeDownInput()) & 0x8001)
				{
					CurPlayer->Translate(RwV3d(0.0f, -0.1f, 0.0f));
				}
			}
		}
	}

	void PlayerMasterSM_Modded::UpdateAmimViewMode()
	{
		const SH::PlayerDebugOptions_Modded& DebugOptions = *SH::PlayerDebugOptions_Modded::GetInstance();
		const bool bWantsAnimViewMode = DebugOptions.IsInAnimViewMode();
		const bool bIsInAnimViewMode = (CurrentState == EActiveState::AnimView);

		// check if the state is mismatched
		if (bIsInAnimViewMode != bWantsAnimViewMode)
		{
			//CurrentState = EActiveState::AnimView;
		}
	}
}