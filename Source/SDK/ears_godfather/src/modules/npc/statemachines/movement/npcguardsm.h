#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCGuardSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCGuardSM() = delete;
		NPCGuardSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCGuardSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x923ADB9E; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCGuardSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum NPCGuardSMStateID : uint32_t
		{
			STATE_INIT                  = 0,
			STATE_FIND_CLOSEST_POINT    = 1,
			STATE_GOTO_CLOSEST_NODE     = 2,
			STATE_GOTO_POINT            = 3,
			STATE_PATH_FAILURE          = 4,
			STATE_PATH_FAILURE_IDLE     = 5,
			STATE_PATH_ULTIMATE_FAILURE = 6,
			STATE_REACHED_GUARD_RANGE   = 7,
			STATE_DONE                  = 8,
			NPCGUARDSM_LAST_STATE       = 9,
		};

	private:
		char m_Padding[0x30];
	};
	static_assert(sizeof(NPCGuardSM) == 0x80);
}
