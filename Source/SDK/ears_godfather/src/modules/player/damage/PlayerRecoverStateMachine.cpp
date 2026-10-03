#include "PlayerRecoverStateMachine.h"

namespace EARS::Modules
{
	PlayerRecoverStateMachine::PlayerRecoverStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerRecoverStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07EAC40, this, TableID, SMParams);
	}

	PlayerRecoverStateMachine::~PlayerRecoverStateMachine()
	{
		MemUtils::CallClassMethod<void, PlayerRecoverStateMachine*>(0x07EACD0, this);
	}

	bool PlayerRecoverStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerRecoverStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07EAD90, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerRecoverStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerRecoverStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07EAF00, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerRecoverStateMachine::S_PlayerRecoverStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerRecoverStateMachine(InID, InSMParams);
	}
}
