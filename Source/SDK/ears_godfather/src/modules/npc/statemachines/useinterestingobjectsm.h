#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class UseInterestingObjectSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		UseInterestingObjectSM() = delete;
		UseInterestingObjectSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~UseInterestingObjectSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x717CEACA; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_UseInterestingObjectSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum UseInterestingObjectSMStateID : uint32_t
		{
			STATE_INIT                = 0,
			STATE_PATH_TO_IO          = 1,
			STATE_REACHED             = 2,
			STATE_ORIENT_TO_OBJECT    = 3,
			STATE_ORIENT_TO_TARGET    = 4,
			STATE_ORIENT_TO_DIR       = 5,
			STATE_WAIT_FOR_LOAD       = 6,
			STATE_TEST_HOLSTER_WEAPON = 7,
			STATE_HOLSTER_WEAPON      = 8,
			STATE_START               = 9,
			STATE_INTERACT            = 10,
			STATE_START_AGAIN         = 11,
			STATE_FINISHING           = 12,
			STATE_RESET               = 13,
			STATE_PATH_FAILURE        = 14,
			STATE_TERMINATE           = 15,
		};

	private:
		char m_Padding[0x48];
	};
	static_assert(sizeof(UseInterestingObjectSM) == 0x98);
}
