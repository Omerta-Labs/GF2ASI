#include "PlayerConcealItemSM.h"

namespace EARS::Modules
{
	PlayerConcealItemSM::PlayerConcealItemSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
	}

	PlayerConcealItemSM::~PlayerConcealItemSM()
	{
		MemUtils::CallClassMethod<void, PlayerConcealItemSM*>(0x07DDAE0, this);
	}

	bool PlayerConcealItemSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerConcealItemSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DDBF0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerConcealItemSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerConcealItemSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07DDC30, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerConcealItemSM::S_PlayerConcealItemSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerConcealItemSM(InID, InSMParams);
	}
}
