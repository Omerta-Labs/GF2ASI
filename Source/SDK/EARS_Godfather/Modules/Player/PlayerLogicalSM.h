#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class PlayerLogicalSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		PlayerLogicalSM() = delete;
		PlayerLogicalSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerLogicalSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x44B1DFAF; }
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerLogicalSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x30];
	};
	static_assert(sizeof(PlayerLogicalSM) == 0x80);
}
