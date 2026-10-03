#pragma once

#include "modules/sentient/statemachines/sentientsm.h"
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class ReloadGunSM : public EARS::Modules::SentientSM
	{
	public:

		ReloadGunSM() = delete;
		ReloadGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~ReloadGunSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x77A4600C; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_ReloadGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};

	class DrawGunSM : public EARS::Modules::SentientSM
	{
	public:

		DrawGunSM() = delete;
		DrawGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~DrawGunSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x3B6DBFA9; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_DrawGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};

	class HolsterGunSM : public EARS::Modules::SentientSM
	{
	public:

		HolsterGunSM() = delete;
		HolsterGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~HolsterGunSM();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x2D48E99B; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_HolsterGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
