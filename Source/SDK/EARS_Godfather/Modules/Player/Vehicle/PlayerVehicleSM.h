#pragma once

#include "SDK/EARS_Godfather/Modules/Player/PlayerSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class PlayerVehicleSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerVehicleSM() = delete;
		PlayerVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerVehicleSM();

		//~ Begin PlayerSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0E76C9833; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End PlayerSM Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x54];
	};
	static_assert(sizeof(PlayerVehicleSM) == 0xC8);
}
