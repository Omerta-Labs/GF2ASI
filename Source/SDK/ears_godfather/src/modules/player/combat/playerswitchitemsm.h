#pragma once

#include "modules/player/playersm.h"
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class PlayerSwitchItemSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerSwitchItemSM() = delete;
		PlayerSwitchItemSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerSwitchItemSM();

		//~ Begin PlayerSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x4C774C1F; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End PlayerSM Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerSwitchItemSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x10];
	};
	static_assert(sizeof(PlayerSwitchItemSM) == 0x84);

	class PlayerInventoryBrowserSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerInventoryBrowserSM() = delete;
		PlayerInventoryBrowserSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerInventoryBrowserSM();

		//~ Begin PlayerSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0DE87E228; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End PlayerSM Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerInventoryBrowserSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);
	};
}
