#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCVaultSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCVaultSM() = delete;
		NPCVaultSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCVaultSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0A276BBEB; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCVaultSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum NPCVaultSMStateID : uint32_t
		{
			STATE_INIT            = 0,
			STATE_APPROACH_VAULT  = 1,
			STATE_PRE_VAULT       = 2,
			STATE_VAULT_WAIT      = 3,
			STATE_VAULT           = 4,
			STATE_COMPLETED_VAULT = 5,
			STATE_REACH_FINAL_POS = 6,
			STATE_VAULT_FAILED    = 7,
			STATE_TERMINATE       = 8,
			NPCVAULTSM_LAST_STATE = 9,
		};

	private:
		char m_Padding[0x98];
	};
	static_assert(sizeof(NPCVaultSM) == 0xE8);
}
