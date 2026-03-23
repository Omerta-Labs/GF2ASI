#pragma once

#include "SDK/EARS_Godfather/Modules/Player/PlayerSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class PlayerVaultSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerVaultSM() = delete;
		PlayerVaultSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerVaultSM();

		//~ Begin PlayerSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0A033CBAB; }
		//~ End PlayerSM Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerVaultSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
