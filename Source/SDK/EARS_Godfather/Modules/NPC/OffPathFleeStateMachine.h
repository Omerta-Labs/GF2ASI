#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/StateMachines/SentientSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class OffPathFleeStateMachine : public EARS::Modules::SentientSM
	{
	public:

		OffPathFleeStateMachine() = delete;
		OffPathFleeStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~OffPathFleeStateMachine();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x996586B0; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_OffPathFleeStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0xA0];
	};
	static_assert(sizeof(OffPathFleeStateMachine) == 0xF8);
}
