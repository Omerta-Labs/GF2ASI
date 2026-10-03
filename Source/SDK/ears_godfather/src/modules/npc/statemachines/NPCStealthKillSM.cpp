#include "NPCStealthKillSM.h"

namespace EARS::Modules
{
	NPCStealthKillSM::NPCStealthKillSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCStealthKillSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07FB9B0, this, TableID, SMParams);
	}

	NPCStealthKillSM::~NPCStealthKillSM()
	{
	}

	EARS::StateMachineSys::StateMachine* NPCStealthKillSM::S_NPCStealthKillSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCStealthKillSM(InID, InSMParams);
	}
}
