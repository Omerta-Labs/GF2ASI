#include "PlayerDeathSM.h"

namespace EARS::Modules
{
	PlayerDeathSM::PlayerDeathSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerDeathSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07E71B0, this, TableID, SMParams);
	}

	PlayerDeathSM::~PlayerDeathSM()
	{
		MemUtils::CallClassMethod<void, PlayerDeathSM*>(0x07E7670, this);
	}

	bool PlayerDeathSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerDeathSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07E7530, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerDeathSM::S_PlayerDeathSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerDeathSM(InID, InSMParams);
	}
}
