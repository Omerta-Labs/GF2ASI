#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class PlayerUpperBodySM : public EARS::Framework::AnimateStateMachine
	{
	public:

		PlayerUpperBodySM() = delete;
		PlayerUpperBodySM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerUpperBodySM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x73B715D2; }
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerUpperBodySM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x28];
	};
	static_assert(sizeof(PlayerUpperBodySM) == 0x78);
}
