#include "turnseeksm.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"
#include "framework/core/memory/globalheapallocator.h"
#include "framework/toolkits/statemachine/smbuilder.h"

namespace EARS::Modules
{
	TurnSeekStateMachine::TurnSeekStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, TurnSeekStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07556C0, this, TableID, SMParams);
	}

	TurnSeekStateMachine::~TurnSeekStateMachine()
	{
		MemUtils::CallClassMethod<void, TurnSeekStateMachine*>(0x04DAF90, this);
	}

	bool TurnSeekStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, TurnSeekStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0756020, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool TurnSeekStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, TurnSeekStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07561D0, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* TurnSeekStateMachine::S_TurnSeekStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new TurnSeekStateMachine(InID, InSMParams);
	}

	//
	// The seven state tables.
	//
	// They split along two axes: what is being tracked (an entity, a position, or the current
	// combat target) and whether reaching it terminates the SM.  The three non-terminating tables
	// are a single state with one update message and no transitions, so the SM runs until its
	// parent tears it down; the four "Terminate" tables add a `done` state that sends
	// MESSAGE_TERMINATE once the heading is close enough.
	//

	/* static */
	void TurnSeekStateMachine::BuildTurnToEntityStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("turnToEntityTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("turnToEntityTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* TurnToEntityState = Builder.AddState("turnToEntity", -1);
		TurnToEntityState->AddUpdateMessage(MESSAGE_TURNTOTARGET);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}

	/* static */
	void TurnSeekStateMachine::BuildTurnToEntityTerminateStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("turnToEntityTerminateTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("turnToEntityTerminateTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* TurnToEntityNoIdleAnimsState = Builder.AddState("turnToEntityNoIdleAnims", -1);
		TurnToEntityNoIdleAnimsState->AddEnterMessage(MESSAGE_DISABLE_IDLE_ANIMS);
		TurnToEntityNoIdleAnimsState->AddEnterMessage(MESSAGE_INIT_TURN_TO_POS_WITH_PROCEDURAL_ROT);
		TurnToEntityNoIdleAnimsState->AddUpdateMessage(MESSAGE_TURNTOTARGET);
		TurnToEntityNoIdleAnimsState->AddTransition("done", TRANSID_NO_TARGET);
		TurnToEntityNoIdleAnimsState->AddTransition("done", TRANSID_TARGETDIRREACHED);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}

	/* static */
	void TurnSeekStateMachine::BuildSetHeadingToEntityStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("setHeadingToEntityTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("setHeadingToEntityTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* SetHeadingToEntityState = Builder.AddState("setHeadingToEntity", -1);
		SetHeadingToEntityState->AddUpdateMessage(MESSAGE_SETHEADINGTOTARGET);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}

	/* static */
	void TurnSeekStateMachine::BuildTurnToPositionTerminateStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("turnToPositionTerminateTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("turnToPositionTerminateTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* TurnToPositionNoIdleAnimsState = Builder.AddState("turnToPositionNoIdleAnims", -1);
		TurnToPositionNoIdleAnimsState->AddEnterMessage(MESSAGE_DISABLE_IDLE_ANIMS);
		TurnToPositionNoIdleAnimsState->AddEnterMessage(MESSAGE_INIT_TURN_TO_POS_WITH_ANIMATED_ROT);
		TurnToPositionNoIdleAnimsState->AddUpdateMessage(MESSAGE_TURNTOTARGETPOS);
		TurnToPositionNoIdleAnimsState->AddTransition("done", TRANSID_TARGETDIRREACHED);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}

	/* static */
	void TurnSeekStateMachine::BuildSeekToPositionTerminateStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("seekToPositionTerminateTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("seekToPositionTerminateTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* SeekToPositionNoAnimsState = Builder.AddState("seekToPositionNoAnims", -1);
		SeekToPositionNoAnimsState->AddUpdateMessage(MESSAGE_TURNTOTARGETPOS_NO_ANIMATIONS);
		// Registered in the shipping build too, not just debug; the handler is expected to no-op
		// when debug drawing is off.
		SeekToPositionNoAnimsState->AddUpdateMessage(MESSAGE_DRAWDEBUG);
		SeekToPositionNoAnimsState->AddTransition("done", TRANSID_TARGETDIRREACHED);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}

	/* static */
	void TurnSeekStateMachine::BuildSeekTargetStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("seekTargetTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("seekTargetTable", get_thread_new_allocator());

		EARS::Framework::SMBuilderState* SeekToTargetNoAnimsState = Builder.AddState("seekToTargetNoAnims", -1);
		SeekToTargetNoAnimsState->AddUpdateMessage(MESSAGE_TURNTOTARGET_NO_ANIMATIONS);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}

	/* static */
	void TurnSeekStateMachine::BuildSeekTargetTerminateStateTable()
	{
		const uint32_t TableID = EARS::StateMachineSys::StateMachineManager::GetStateTableIDFromName("seekTargetTerminateTable");
		if (EARS::StateMachineSys::StateMachineManager::GetInstance()->GetStateTableFromID(TableID) != nullptr)
		{
			return;
		}

		EARS::Framework::SMBuilder Builder("seekTargetTerminateTable", get_thread_new_allocator());

		// Same state name as seekTargetTable's, but this copy terminates once the heading is
		// reached or the target is gone.
		EARS::Framework::SMBuilderState* SeekToTargetNoAnimsState = Builder.AddState("seekToTargetNoAnims", -1);
		SeekToTargetNoAnimsState->AddUpdateMessage(MESSAGE_TURNTOTARGET_NO_ANIMATIONS);
		SeekToTargetNoAnimsState->AddTransition("done", TRANSID_NO_TARGET);
		SeekToTargetNoAnimsState->AddTransition("done", TRANSID_TARGETDIRREACHED);

		EARS::Framework::SMBuilderState* DoneState = Builder.AddState("done", -1);
		DoneState->AddEnterMessage(EARS::StateMachineSys::State::MESSAGE_TERMINATE);

		Builder.CompileAndRegister(kStateMachine_TurnSeekStateMachine, S_TurnSeekStateMachine_FactoryFn, "TurnSeekStateMachine");
	}
}
