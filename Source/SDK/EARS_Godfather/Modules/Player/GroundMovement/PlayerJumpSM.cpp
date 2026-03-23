#include "PlayerJumpSM.h"

namespace EARS::Modules
{
	PlayerJumpSM::PlayerJumpSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerJumpSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07D1940, this, TableID, SMParams);
	}

	PlayerJumpSM::~PlayerJumpSM()
	{
		MemUtils::CallClassMethod<void, PlayerJumpSM*>(0x07D1A10, this);
	}

	bool PlayerJumpSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerJumpSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07D1FD0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerJumpSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerJumpSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07D1C40, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerJumpSM::S_PlayerJumpSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerJumpSM(InID, InSMParams);
	}
}
