#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class CombatPathSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		CombatPathSM() = delete;
		CombatPathSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~CombatPathSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x6D994273; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_CombatPathSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum CombatPathSMStateID : uint32_t
		{
			STATE_INIT          = 0,
			STATE_STRAFE_ATTACK = 1,
			STATE_RUN           = 2,
			STATE_SMOOTHER      = 3,
			STATE_TERMINATE     = 4,
		};

	private:
		char m_Padding[0x38];
	};
	static_assert(sizeof(CombatPathSM) == 0x88);
}
