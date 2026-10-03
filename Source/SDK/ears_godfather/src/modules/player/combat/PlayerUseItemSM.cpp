#include "PlayerUseItemSM.h"

namespace EARS::Modules
{
	PlayerUseItemSM::PlayerUseItemSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerUseItemSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07DF900, this, TableID, SMParams);
	}

	PlayerUseItemSM::~PlayerUseItemSM()
	{
	}

	bool PlayerUseItemSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerUseItemSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07E00A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerUseItemSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerUseItemSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07E03E0, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerUseItemSM::S_PlayerUseItemSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerUseItemSM(InID, InSMParams);
	}
}
