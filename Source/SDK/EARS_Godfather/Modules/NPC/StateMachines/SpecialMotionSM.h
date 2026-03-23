#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/StateMachines/SentientSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class SpecialMotionSM : public EARS::Modules::SentientSM
	{
	public:

		SpecialMotionSM() = delete;
		SpecialMotionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~SpecialMotionSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x10CE9029; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_SpecialMotionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0xB8];
	};
	static_assert(sizeof(SpecialMotionSM) == 0x110);
}
