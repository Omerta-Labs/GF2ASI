#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class NPCSitObjectSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCSitObjectSM() = delete;
		NPCSitObjectSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCSitObjectSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x1E9BA656; }
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCSitObjectSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
