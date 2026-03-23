#pragma once

// SDK
#include "SDK/EARS_Godfather/Modules/Player/PlayerMasterSM.h"

namespace SH
{
	/**
	 * Modded state machine to cover the bespoke functionality added by scripthook
	 */
	class PlayerMasterSM_Modded : public EARS::Modules::PlayerMasterSM
	{
	public:

		PlayerMasterSM_Modded() = delete;
		PlayerMasterSM_Modded(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams);
		virtual ~PlayerMasterSM_Modded();

		//~ Begin EARS::Modules::PlayerSM interface
		virtual uint32_t GetStateMachineID() const override;
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual void InitialiseChild(StateMachine& ChildMachine) override;
		//~ End  EARS::Modules::PlayerSM interface

	private:

		enum class EActiveState
		{
			Game,
			FlyMode,
			AnimView
		};

		/** not part of game, this is for SDK */
		void UpdateFlyMode();

		/** not part of game, this is for SDK */
		void UpdateAmimViewMode();

		EActiveState CurrentState = EActiveState::Game;
	};
}
