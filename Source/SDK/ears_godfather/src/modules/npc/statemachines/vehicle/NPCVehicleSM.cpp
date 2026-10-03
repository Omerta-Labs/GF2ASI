#include "NPCVehicleSM.h"

namespace EARS::Modules
{
	NPCVehicleSM::NPCVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
	}

	NPCVehicleSM::~NPCVehicleSM()
	{
	}

	EARS::StateMachineSys::StateMachine* NPCVehicleSM::S_NPCVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCVehicleSM(InID, InSMParams);
	}
}
