#include "PlayerRevealItemSM.h"

namespace EARS::Modules
{
	PlayerRevealItemSM::PlayerRevealItemSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
	}

	PlayerRevealItemSM::~PlayerRevealItemSM()
	{
	}

	bool PlayerRevealItemSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerRevealItemSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DE4E0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerRevealItemSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerRevealItemSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07DE320, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerRevealItemSM::S_PlayerRevealItemSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerRevealItemSM(InID, InSMParams);
	}
}
