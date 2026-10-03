#pragma once

#include "modules/sentient/statemachines/sentientsm.h"
#include "Addons/Hook.h"
#include "framework/toolkits/statemachine/animatesm.h"

namespace EARS::Modules
{
	class AmbushSM : public EARS::Modules::SentientSM
	{
	public:

		AmbushSM() = delete;
		AmbushSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~AmbushSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x9C39BACE; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_AmbushSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};

	class NPCRandomSearchSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCRandomSearchSM() = delete;
		NPCRandomSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCRandomSearchSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0CF840186; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCRandomSearchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x38];
	};
	static_assert(sizeof(NPCRandomSearchSM) == 0x88);

	class NPCSearchSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		NPCSearchSM() = delete;
		NPCSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~NPCSearchSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0E653FF63; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_NPCSearchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x78];
	};
	static_assert(sizeof(NPCSearchSM) == 0xC8);
}
