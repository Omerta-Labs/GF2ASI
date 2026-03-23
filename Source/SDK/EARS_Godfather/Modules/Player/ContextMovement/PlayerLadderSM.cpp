#include "PlayerLadderSM.h"

namespace EARS::Modules
{
	PlayerLadderSM::PlayerLadderSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerLadderSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C2300, this, TableID, SMParams);
	}

	PlayerLadderSM::~PlayerLadderSM()
	{
		MemUtils::CallClassMethod<void, PlayerLadderSM*>(0x07C2080, this);
	}

	bool PlayerLadderSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerLadderSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07C2200, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerLadderSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerLadderSM*, EARS::StateMachineSys::StateMachine*>(0x07C21B0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerLadderSM::S_PlayerLadderSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerLadderSM(InID, InSMParams);
	}
}
