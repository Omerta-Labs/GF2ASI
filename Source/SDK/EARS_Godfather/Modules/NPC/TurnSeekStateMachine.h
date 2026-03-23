#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/StateMachines/SentientSM.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class TurnSeekStateMachine : public EARS::Modules::SentientSM
	{
	public:

		TurnSeekStateMachine() = delete;
		TurnSeekStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~TurnSeekStateMachine();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x1B2D5C51; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_TurnSeekStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x2C];
	};
	static_assert(sizeof(TurnSeekStateMachine) == 0x84);
}
