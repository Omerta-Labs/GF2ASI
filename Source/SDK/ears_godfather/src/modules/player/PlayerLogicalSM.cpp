#include "PlayerLogicalSM.h"

namespace EARS::Modules
{
	PlayerLogicalSM::PlayerLogicalSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerLogicalSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07AB150, this, TableID, SMParams);
	}

	PlayerLogicalSM::~PlayerLogicalSM()
	{
	}

	EARS::StateMachineSys::StateMachine* PlayerLogicalSM::S_PlayerLogicalSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerLogicalSM(InID, InSMParams);
	}
}
