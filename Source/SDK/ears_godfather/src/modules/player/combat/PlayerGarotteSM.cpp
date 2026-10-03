#include "PlayerGarotteSM.h"

namespace EARS::Modules
{
	PlayerGarotteSM::PlayerGarotteSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerGarotteSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07AE120, this, TableID, SMParams);
	}

	PlayerGarotteSM::~PlayerGarotteSM()
	{
		MemUtils::CallClassMethod<void, PlayerGarotteSM*>(0x07AE1C0, this);
	}

	bool PlayerGarotteSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGarotteSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07AEA70, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerGarotteSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGarotteSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07AE240, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerGarotteSM::S_PlayerGarotteSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerGarotteSM(InID, InSMParams);
	}
}
