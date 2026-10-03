#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class ScriptedSequenceSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		ScriptedSequenceSM() = delete;
		ScriptedSequenceSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ScriptedSequenceSM();

		virtual uint32_t GetStateMachineID() const override { return 0x6DE5FFD8; }

		static EARS::StateMachineSys::StateMachine* S_ScriptedSequenceSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
