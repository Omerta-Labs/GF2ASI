#include "tracktoposanddirsm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	TrackToPositionAndDirectionSM::TrackToPositionAndDirectionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, TrackToPositionAndDirectionSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0754AE0, this, TableID, SMParams);
	}

	TrackToPositionAndDirectionSM::~TrackToPositionAndDirectionSM()
	{
		MemUtils::CallClassMethod<void, TrackToPositionAndDirectionSM*>(0x04AC1E0, this);
	}

	bool TrackToPositionAndDirectionSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, TrackToPositionAndDirectionSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0755510, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool TrackToPositionAndDirectionSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, TrackToPositionAndDirectionSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0754BC0, this, SimTime, FrameTime, TransID, TransData);
	}

	int TrackToPositionAndDirectionSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, TrackToPositionAndDirectionSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* TrackToPositionAndDirectionSM::S_TrackToPositionAndDirectionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new TrackToPositionAndDirectionSM(InID, InSMParams);
	}

	/* static */
	void TrackToPositionAndDirectionSM::BuildStateMachine()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("trackToPositionAndDirectionStateTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("trackToPositionAndDirectionStateTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* TrackingState = Builder.AddState("tracking", -1);
		TrackingState->AddUpdateMessage(MESSAGE_UPDATETRACKING);
		TrackingState->AddTransition("exit", TRANSID_DONETRACKING);
		TrackingState->AddTransition("exit", TRANSID_IS_GHOST);

		EARS::Framework::SMBuilderState* ExitState = Builder.AddState("exit", -1);
		ExitState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_TrackToPositionAndDirectionSM, S_TrackToPositionAndDirectionSM_FactoryFn, "TrackToPositionAndDirectionSM");
	}
}
