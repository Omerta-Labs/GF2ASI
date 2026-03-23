#include "PlayerVaultSM.h"

namespace EARS::Modules
{
	PlayerVaultSM::PlayerVaultSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
	}

	PlayerVaultSM::~PlayerVaultSM()
	{
	}

	EARS::StateMachineSys::StateMachine* PlayerVaultSM::S_PlayerVaultSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerVaultSM(InID, InSMParams);
	}
}
