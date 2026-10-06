#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class NPCCollideSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCCollideSM() = delete;
		NPCCollideSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCCollideSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x408C7112; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCCollideSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum NPCCollideSMStateID : uint32_t
		{
			STATE_INIT              = 0,
			STATE_ORIENT_TO_COLLIDE = 1,
			STATE_PLAY_COLLIDE      = 2,
		};

	private:
		char m_Padding[0x28];
	};
	static_assert(sizeof(NPCCollideSM) == 0x78);
}
