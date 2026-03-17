#include "PlayerAnimViewSM.h"

// Scripthook
#include "Scripthook/SH_PlayerMasterSM/PlayerDebugOptions_Modded.h"
// SDK
#include <SDK/EARS_Common/CommonTypes.h>
#include <SDK/EARS_Framework/Toolkits/StateMachine/SMBuilder.h>

// Addons
#include "Addons/Hook.h"

namespace Private
{
	const uint32_t ANIM_VEW_SM_HASH = EARS::Common::HashMem_SDBM("PlayerAnimViewSM", 17);

	EARS::StateMachineSys::StateMachine* S_PlayerAnimViewSM_FactoryFn(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
	{
		return new SH::PlayerAnimViewSM(id, pSMParams);
	}
}

namespace SH
{
	PlayerAnimViewSM::PlayerAnimViewSM(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams)
		: EARS::Modules::PlayerSM(TableID, SmParams)
	{
	}

	PlayerAnimViewSM::~PlayerAnimViewSM()
	{
	}

	uint32_t PlayerAnimViewSM::GetStateMachineID() const
	{
		return Private::ANIM_VEW_SM_HASH;
	}

	bool PlayerAnimViewSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return EARS::Modules::PlayerSM::HandleStateMessage(SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerAnimViewSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		if (TransID == 0x11)
		{
			// should we enter anim view mode
			SH::PlayerDebugOptions_Modded& DebugOptions = *SH::PlayerDebugOptions_Modded::GetInstance();
			return (DebugOptions.IsInAnimViewMode() == false);
		}

		// call parent class
		return EARS::Modules::PlayerSM::CheckTransition(SimTime, FrameTime, TransID, TransData);
	}

	/* static */
	void PlayerAnimViewSM::BuildStateMachine()
	{
		// TODO: Check whether the state machine has already been built!
		void* thread_new_allocator = MemUtils::CallCdeclMethod<void*>(0x09C8F80);
		EARS::Framework::SMBuilder Builder = EARS::Framework::SMBuilder::MakeBuilder("playerDebugAnimViewStateTable", thread_new_allocator);

		// IDLE STATE
		EARS::Framework::SMBuilderState* IdleState = Builder.AddState("idle", -1);
		IdleState->AddTransition("exit", 0x11);

		// EXIT STATE
		EARS::Framework::SMBuilderState* ExitState = Builder.AddState("exit", -1);
		ExitState->AddEnterMessage(0x2);

		Builder.CompileAndRegister(Private::ANIM_VEW_SM_HASH, Private::S_PlayerAnimViewSM_FactoryFn, "PlayerAnimViewSM");

		Builder.Destroy();
	}
}
