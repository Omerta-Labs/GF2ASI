#include "PlayerTargetingSM.h"

namespace EARS::Modules
{
	PlayerTargetingSM::PlayerTargetingSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerTargetingSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07E5780, this, TableID, SMParams);
	}

	PlayerTargetingSM::~PlayerTargetingSM()
	{
		MemUtils::CallClassMethod<void, PlayerTargetingSM*>(0x07E5820, this);
	}

	bool PlayerTargetingSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerTargetingSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07E6D60, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerTargetingSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerTargetingSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07E5D00, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerTargetingSM::S_PlayerTargetingSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerTargetingSM(InID, InSMParams);
	}
}
