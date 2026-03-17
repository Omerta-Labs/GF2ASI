#pragma once

// SDK
#include <SDK/EARS_Godfather/Modules/Player/PlayerSM.h>

namespace SH
{
	class PlayerAnimViewSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerAnimViewSM() = delete;
		PlayerAnimViewSM(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams);
		virtual ~PlayerAnimViewSM();

		//~ Begin EARS::Modules::PlayerSM interface
		virtual uint32_t GetStateMachineID() const override;
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End  EARS::Modules::PlayerSM interface

		static void BuildStateMachine();

	private:
	};
}
