#pragma once

// CPP
#include "SDK/EARS_Godfather/Modules/Player/PlayerSM.h"

namespace EARS
{
	namespace Modules
	{
		class PlayerMasterSM : public EARS::Modules::PlayerSM
		{
		public:

			PlayerMasterSM() = delete;
			PlayerMasterSM(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams);
			virtual ~PlayerMasterSM();

			//~ Begin EARS::Modules::PlayerSM interface
			virtual uint32_t GetStateMachineID() const override;
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData) override;
			virtual void InitialiseChild(StateMachine* ChildMachine) override;
			//~ End  EARS::Modules::PlayerSM interface
		};

		static_assert(sizeof(PlayerMasterSM) == 0x74);
	}
}
