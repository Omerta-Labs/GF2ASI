#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class PlayerLungeAttackSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		PlayerLungeAttackSM() = delete;
		PlayerLungeAttackSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerLungeAttackSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x2D5CBC4; }
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerLungeAttackSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x28];
	};
	static_assert(sizeof(PlayerLungeAttackSM) == 0x78);
}
