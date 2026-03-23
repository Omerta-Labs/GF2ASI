#pragma once

#include "SDK/EARS_Godfather/Modules/Player/PlayerSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class PlayerVehicleHijackSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerVehicleHijackSM() = delete;
		PlayerVehicleHijackSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerVehicleHijackSM();

		//~ Begin PlayerSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x1883C985; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End PlayerSM Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerVehicleHijackSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x20];
	};
	static_assert(sizeof(PlayerVehicleHijackSM) == 0x94);
}
