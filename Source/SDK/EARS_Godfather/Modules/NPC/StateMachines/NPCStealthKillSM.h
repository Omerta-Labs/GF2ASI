#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class NPCStealthKillSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCStealthKillSM() = delete;
		NPCStealthKillSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCStealthKillSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0E4B1F57A; }
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCStealthKillSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x28];
	};
	static_assert(sizeof(NPCStealthKillSM) == 0x78);
}
