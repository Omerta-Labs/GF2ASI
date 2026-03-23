#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class NPCVehicleSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCVehicleSM() = delete;
		NPCVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCVehicleSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0EE359985; }
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
