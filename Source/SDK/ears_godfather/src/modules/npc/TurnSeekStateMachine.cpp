#include "TurnSeekStateMachine.h"

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
}
