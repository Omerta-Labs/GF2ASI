#include "PlayerCarBombSM.h"

namespace EARS::Modules
{
	PlayerCarBombSM::PlayerCarBombSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
	}

	PlayerCarBombSM::~PlayerCarBombSM()
	{
	}

	bool PlayerCarBombSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerCarBombSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DD980, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerCarBombSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerCarBombSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07DD700, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerCarBombSM::S_PlayerCarBombSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerCarBombSM(InID, InSMParams);
	}
}
