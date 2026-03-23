#include "PlayerPlantSM.h"

namespace EARS::Modules
{
	PlayerPlantSM::PlayerPlantSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerPlantSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C2F90, this, TableID, SMParams);
	}

	PlayerPlantSM::~PlayerPlantSM()
	{
		MemUtils::CallClassMethod<void, PlayerPlantSM*>(0x07C2EE0, this);
	}

	bool PlayerPlantSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerPlantSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C34A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerPlantSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerPlantSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07C3670, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerPlantSM::S_PlayerPlantSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerPlantSM(InID, InSMParams);
	}
}
