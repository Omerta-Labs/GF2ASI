#pragma once

// CPP
#include "SDK/EARS_Godfather/Modules/Sentient/StateMachines/SentientSM.h"

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class Player;
		class PlayerActionableInfo;
		struct PlayerCameraInfo;
		class PlayerF2FInfo;
		class PlayerLogicalInfo;
		class PlayerUpperBodyInfo;


		class PlayerSM : public EARS::Modules::SentientSM
		{
		public:

			PlayerSM() = delete;
			PlayerSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
			virtual ~PlayerSM();

			//~ Begin EARS::Modules::SentientSM interface
			virtual uint32_t GetStateMachineID() const override;
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
			//~ End  EARS::Modules::SentientSM interface

			// getters
			Player* GetPlayer() const { return m_Player; }
			PlayerUpperBodyInfo* GetUpperBodyInfo() const { return m_UpperBodyInfo; }
			PlayerLogicalInfo* GetLogicalInfo() const { return m_LogicalInfo; }
			PlayerF2FInfo* GetF2FInfo() const { return m_F2FInfo; }
			PlayerActionableInfo* GetActionableInfo() const { return m_ActionableInfo; }
			PlayerCameraInfo* GetCameraInfo() const { return m_CameraInfo; }

		private:

			EARS::Modules::Player* m_Player = nullptr;										// 0x58
			EARS::Modules::PlayerUpperBodyInfo* m_UpperBodyInfo = nullptr;					// 0x5C
			EARS::Modules::PlayerLogicalInfo* m_LogicalInfo = nullptr;						// 0x60
			EARS::Modules::PlayerF2FInfo* m_F2FInfo = nullptr;								// 0x64
			EARS::Modules::PlayerActionableInfo* m_ActionableInfo = nullptr;				// 0x68
			EARS::Modules::PlayerCameraInfo* m_CameraInfo = nullptr;						// 0x6C
			bool m_bPairedAnimSwitched = false;												// 0x70
		};
	}
}
