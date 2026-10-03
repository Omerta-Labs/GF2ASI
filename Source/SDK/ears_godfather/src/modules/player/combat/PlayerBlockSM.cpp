#include "PlayerBlockSM.h"

namespace EARS::Modules
{
	PlayerBlockSM::PlayerBlockSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerBlockSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07AD870, this, TableID, SMParams);
	}

	PlayerBlockSM::~PlayerBlockSM()
	{
		MemUtils::CallClassMethod<void, PlayerBlockSM*>(0x07AB150, this);
	}

	bool PlayerBlockSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerBlockSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07AD930, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerBlockSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerBlockSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07ADDE0, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerBlockSM::S_PlayerBlockSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerBlockSM(InID, InSMParams);
	}
}
