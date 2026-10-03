#include "PlayerUseDisposableItemSM.h"

namespace EARS::Modules
{
	PlayerUseDisposableItemSM::PlayerUseDisposableItemSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
	}

	PlayerUseDisposableItemSM::~PlayerUseDisposableItemSM()
	{
		MemUtils::CallClassMethod<void, PlayerUseDisposableItemSM*>(0x07DF270, this);
	}

	bool PlayerUseDisposableItemSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerUseDisposableItemSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DF7A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerUseDisposableItemSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerUseDisposableItemSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07DF800, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerUseDisposableItemSM::S_PlayerUseDisposableItemSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerUseDisposableItemSM(InID, InSMParams);
	}
}
