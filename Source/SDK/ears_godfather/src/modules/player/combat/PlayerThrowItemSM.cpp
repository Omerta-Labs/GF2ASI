#include "PlayerThrowItemSM.h"

namespace EARS::Modules
{
	PlayerThrowItemSM::PlayerThrowItemSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerThrowItemSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07DEEA0, this, TableID, SMParams);
	}

	PlayerThrowItemSM::~PlayerThrowItemSM()
	{
		MemUtils::CallClassMethod<void, PlayerThrowItemSM*>(0x07AB150, this);
	}

	bool PlayerThrowItemSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerThrowItemSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DF010, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	EARS::StateMachineSys::StateMachine* PlayerThrowItemSM::S_PlayerThrowItemSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerThrowItemSM(InID, InSMParams);
	}
}
