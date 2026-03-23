#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/StateMachines/SentientSM.h"

// Addons
#include "Addons/Hook.h"

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

	private:
		char m_Padding[0x24];
	};
	static_assert(sizeof(NPCFollowSM) == 0x7C);
}
