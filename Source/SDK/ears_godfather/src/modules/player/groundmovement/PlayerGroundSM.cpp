#include "PlayerGroundSM.h"

namespace EARS::Modules
{
	PlayerGroundSM::PlayerGroundSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerGroundSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07CFC90, this, TableID, SMParams);
	}

	PlayerGroundSM::~PlayerGroundSM()
	{
		MemUtils::CallClassMethod<void, PlayerGroundSM*>(0x07D00E0, this);
	}

	bool PlayerGroundSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGroundSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07D1680, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerGroundSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGroundSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07D0E00, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerGroundSM::S_PlayerGroundSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerGroundSM(InID, InSMParams);
	}
}
