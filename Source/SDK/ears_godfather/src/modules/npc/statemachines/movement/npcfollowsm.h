#pragma once

#include "modules/sentient/statemachines/sentientsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCFollowSM : public EARS::Modules::SentientSM
	{
	public:

		NPCFollowSM() = delete;
		NPCFollowSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCFollowSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x8E2DEEEC; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_NPCFollowSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum NPCFollowSMStateID : uint32_t
		{
			STATE_INIT         = 0,
			STATE_STAND_FOLLOW = 1,
			STATE_SEEK_FAST    = 2,
			STATE_STRAFE_FAST  = 3,
			STATE_SEEK_SLOW    = 4,
			STATE_BACKUP       = 5,
			STATE_PATH_FAILURE = 6,
		};

	private:
		char m_Padding[0x24];
	};
	static_assert(sizeof(NPCFollowSM) == 0x7C);
}
