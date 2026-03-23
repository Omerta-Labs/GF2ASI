#pragma once

// CPP
#include "SDK/EARS_Godfather/Modules/Player/PlayerSM.h"

namespace EARS
{
	namespace Modules
	{
		// TODO: Consider removing fly debug SM from this SM.
		// It was initially desired by players but they ultimately decided on a different fly mode.
		// We have a derived version of this SM now, which implements fly mode

		/**
		 * Master State Machine for the Player in Godfather II.
		 */
		class PlayerMasterSM : public EARS::Modules::PlayerSM
		{
		public:

			PlayerMasterSM() = delete;
			PlayerMasterSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
			virtual ~PlayerMasterSM();

			//~ Begin EARS::Modules::PlayerSM interface
			virtual uint32_t GetStateMachineID() const override;
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
			virtual void InitialiseChild(StateMachine& ChildMachine) override;
			//~ End  EARS::Modules::PlayerSM interface
		};

		static_assert(sizeof(PlayerMasterSM) == 0x74);
	}
}
