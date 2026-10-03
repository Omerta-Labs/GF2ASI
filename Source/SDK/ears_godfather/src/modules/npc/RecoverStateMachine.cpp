#include "RecoverStateMachine.h"

namespace EARS::Modules
{
	RecoverStateMachine::RecoverStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, RecoverStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0771EC0, this, TableID, SMParams);
	}

	RecoverStateMachine::~RecoverStateMachine()
	{
		MemUtils::CallClassMethod<void, RecoverStateMachine*>(0x0771F40, this);
	}

	bool RecoverStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, RecoverStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0771FD0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool RecoverStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, RecoverStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0771C70, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* RecoverStateMachine::S_RecoverStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new RecoverStateMachine(InID, InSMParams);
	}
}
