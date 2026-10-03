#include "PlayerUpperBodySM.h"

namespace EARS::Modules
{
	PlayerUpperBodySM::PlayerUpperBodySM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerUpperBodySM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07AB130, this, TableID, SMParams);
	}

	PlayerUpperBodySM::~PlayerUpperBodySM()
	{
	}

	EARS::StateMachineSys::StateMachine* PlayerUpperBodySM::S_PlayerUpperBodySM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerUpperBodySM(InID, InSMParams);
	}
}
