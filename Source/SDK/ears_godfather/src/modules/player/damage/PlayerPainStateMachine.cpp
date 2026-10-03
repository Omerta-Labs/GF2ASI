#include "PlayerPainStateMachine.h"

namespace EARS::Modules
{
	PlayerPainStateMachine::PlayerPainStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerPainStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07E9890, this, TableID, SMParams);
	}

	PlayerPainStateMachine::~PlayerPainStateMachine()
	{
		MemUtils::CallClassMethod<void, PlayerPainStateMachine*>(0x07E9930, this);
	}

	bool PlayerPainStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerPainStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07EA9C0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerPainStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerPainStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07E99E0, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerPainStateMachine::S_PlayerPainStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerPainStateMachine(InID, InSMParams);
	}
}
