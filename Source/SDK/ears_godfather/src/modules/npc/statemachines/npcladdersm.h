#pragma once

#include "modules/sentient/statemachines/sentientsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCLadderSM : public EARS::Modules::SentientSM
	{
	public:

		NPCLadderSM() = delete;
		NPCLadderSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCLadderSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x5BE975D; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_NPCLadderSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum NPCLadderSMStateID : uint32_t
		{
			STATE_INIT                     = 0,
			STATE_APPROACH_LADDER          = 1,
			STATE_APPROACH_LADDER_FAILED   = 2,
			STATE_ENTER_LADDER             = 3,
			STATE_LADDER_CLIMB_WAIT        = 4,
			STATE_LADDER_CLIMB             = 5,
			STATE_LADDER_DEATH             = 6,
			STATE_COMPLETED_LADDER_CLIMB   = 7,
			STATE_REACH_FINAL_POS          = 8,
			STATE_SEEK_TO_FINAL_POS_FAILED = 9,
			STATE_LADDER_CLIMB_FAILED      = 10,
			STATE_TERMINATE                = 11,
			NPCLADDERCLIMBSM_LAST_STATE    = 12,
		};

	private:
		char m_Padding[0x54];
	};
	static_assert(sizeof(NPCLadderSM) == 0xAC);
}
