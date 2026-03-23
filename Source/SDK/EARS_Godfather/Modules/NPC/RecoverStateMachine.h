#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/StateMachines/SentientSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class RecoverStateMachine : public EARS::Modules::SentientSM
	{
	public:

		RecoverStateMachine() = delete;
		RecoverStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~RecoverStateMachine();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x1C07B25; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_RecoverStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x14];
	};
	static_assert(sizeof(RecoverStateMachine) == 0x6C);
}
