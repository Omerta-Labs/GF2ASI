#include "ScriptedSequenceSM.h"

namespace EARS::Modules
{
	ScriptedSequenceSM::ScriptedSequenceSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
	}

	ScriptedSequenceSM::~ScriptedSequenceSM()
	{
	}

	EARS::StateMachineSys::StateMachine* ScriptedSequenceSM::S_ScriptedSequenceSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ScriptedSequenceSM(InID, InSMParams);
	}
}
