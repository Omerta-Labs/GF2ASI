#include "NPCSitObjectSM.h"

namespace EARS::Modules
{
	NPCSitObjectSM::NPCSitObjectSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
	}

	NPCSitObjectSM::~NPCSitObjectSM()
	{
	}

	EARS::StateMachineSys::StateMachine* NPCSitObjectSM::S_NPCSitObjectSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCSitObjectSM(InID, InSMParams);
	}
}
