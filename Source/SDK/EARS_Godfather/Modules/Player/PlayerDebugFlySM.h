#pragma once

// CPP
#include "SDK/EARS_Godfather/Modules/Player/PlayerSM.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * A reimplementation of the 'PlayerDebugFlySM' StateMachine found in Godfather II dev builds.
		 * To make this work and included in the PlayerMasterSM, you need to inject some code into the SMBuilder.
		 * See HookMods.cpp for more, but this is generally outside of the SDK remit as it is still theoretically mod code.
		 */
		class PlayerDebugFlySM : public EARS::Modules::PlayerSM
		{
		public:

			PlayerDebugFlySM() = delete;
			PlayerDebugFlySM(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams);
			virtual ~PlayerDebugFlySM();

			//~ Begin EARS::Modules::PlayerSM interface
			virtual uint32_t GetStateMachineID() const override;
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData) override;
			//~ End  EARS::Modules::PlayerSM interface

			static void BuildStateMachine();

		private:

			void ProcessMovement(float FrameTime);
		};
	}
}
