#pragma once

#include "framework/toolkits/statemachine/animatesm.h"
#include "Platform/MemUtils.h"
#include "modules/sentient/statemachines/sentientsm.h"

namespace EARS::Modules
{
	class ShootSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		ShootSM() = delete;
		ShootSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ShootSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x44BE8AE2; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_ShootSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x40];
	};
	static_assert(sizeof(ShootSM) == 0x90);

	class ContinualBurstFireSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		ContinualBurstFireSM() = delete;
		ContinualBurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ContinualBurstFireSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x5EA8C0E7; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_ContinualBurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x28];
	};
	static_assert(sizeof(ContinualBurstFireSM) == 0x78);

	class BurstFireSM : public EARS::Modules::SentientSM
	{
	public:

		BurstFireSM() = delete;
		BurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~BurstFireSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x211BC9C1; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_BurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
