#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class MustFollowPathStateMachine : public EARS::Framework::AnimateStateMachine
	{
	public:

		MustFollowPathStateMachine() = delete;
		MustFollowPathStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~MustFollowPathStateMachine();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0E7934180; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_MustFollowPathStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x5C];
	};
	static_assert(sizeof(MustFollowPathStateMachine) == 0xAC);
}
