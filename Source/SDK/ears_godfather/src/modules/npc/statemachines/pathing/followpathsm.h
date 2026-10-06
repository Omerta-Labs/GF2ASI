#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class FollowPathStateMachine : public EARS::Framework::AnimateStateMachine
	{
	public:

		FollowPathStateMachine() = delete;
		FollowPathStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~FollowPathStateMachine();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x6862A74C; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_FollowPathStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum FollowPathSMStateID : uint32_t
		{
			STATE_INIT                    = 0,
			STATE_CLEAR_STRAFE_N_REORIENT = 1,
			STATE_SEEK_TO_START_NODE      = 2,
			STATE_FIND_PATH               = 3,
			STATE_FIND_PATH_FAILURE       = 4,
			STATE_INIT_GRAPHCLIENT        = 5,
			STATE_FOLLOW_GRAPHCLIENT      = 6,
			STATE_SEEK_TO_SPECIAL_NODE    = 7,
			STATE_ARRIVED_AT_SPECIAL_NODE = 8,
			STATE_POST_SPECIAL_NODE       = 9,
			STATE_FOLLOW_GRAPH_IDLE       = 10,
			STATE_SEEK_TO_TARGET          = 11,
			STATE_WAIT_GEO                = 12,
			STATE_BLOCKED                 = 13,
			STATE_NO_PATH_DONE            = 14,
			STATE_DONE                    = 15,
			FOLLOWPATHSM_LAST_STATE       = 16,
		};


	protected:

		//~ Begin AnimateStateMachine Interface
		virtual const EARS::StateMachineSys::StateMachineSnapshot* ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		virtual EARS::StateMachineSys::StateMachineSnapshot* WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End AnimateStateMachine Interface

	private:
		char m_Padding[0x114];
	};
	static_assert(sizeof(FollowPathStateMachine) == 0x164);
}
