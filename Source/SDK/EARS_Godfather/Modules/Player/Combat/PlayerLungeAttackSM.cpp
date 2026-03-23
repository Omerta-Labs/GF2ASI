#include "PlayerLungeAttackSM.h"

namespace EARS::Modules
{
	PlayerLungeAttackSM::PlayerLungeAttackSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerLungeAttackSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07AB540, this, TableID, SMParams);
	}

	PlayerLungeAttackSM::~PlayerLungeAttackSM()
	{
	}

	EARS::StateMachineSys::StateMachine* PlayerLungeAttackSM::S_PlayerLungeAttackSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerLungeAttackSM(InID, InSMParams);
	}
}
