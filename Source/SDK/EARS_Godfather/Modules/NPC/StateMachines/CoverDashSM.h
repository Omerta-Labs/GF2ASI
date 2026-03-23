#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class CoverDashSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		CoverDashSM() = delete;
		CoverDashSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CoverDashSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x14CBC763; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CoverDashSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x38];
	};
	static_assert(sizeof(CoverDashSM) == 0x88);
}
