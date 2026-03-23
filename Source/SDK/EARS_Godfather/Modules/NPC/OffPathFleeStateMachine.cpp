#include "OffPathFleeStateMachine.h"

namespace EARS::Modules
{
	OffPathFleeStateMachine::OffPathFleeStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, OffPathFleeStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x074D3B0, this, TableID, SMParams);
	}

	OffPathFleeStateMachine::~OffPathFleeStateMachine()
	{
		MemUtils::CallClassMethod<void, OffPathFleeStateMachine*>(0x09C8EB0, this);
	}

	bool OffPathFleeStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, OffPathFleeStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x06E6C20, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	EARS::StateMachineSys::StateMachine* OffPathFleeStateMachine::S_OffPathFleeStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new OffPathFleeStateMachine(InID, InSMParams);
	}
}
