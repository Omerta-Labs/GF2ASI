#include "PlayerSM.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

// Addons
#include "Addons/Hook.h"

namespace EARS
{
	namespace Modules
	{
		PlayerSM::PlayerSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
			: EARS::Modules::SentientSM(TableID, SMParams)
			, m_bPairedAnimSwitched(false)
		{
			// need to assign as player if possible
			EARS::Framework::Animated* AnimEntity = m_AnimatedEntity.GetPtr();
			m_Player = EARS::Framework::_GetInterface<EARS::Modules::Player>(AnimEntity, 0x383225A1u);
		
			// now get all behaviours from this player
			m_UpperBodyInfo = m_Player->GetUpperBodyInfo();
			m_LogicalInfo = m_Player->GetLogicalInfo();
			m_ActionableInfo = m_Player->GetActionableInfo();
			m_CameraInfo = m_Player->GetCameraInfo();
			m_F2FInfo = m_Player->GetF2FInfo();
		}

		PlayerSM::~PlayerSM()
		{
			 // nothing implemented
		}

		uint32_t PlayerSM::GetStateMachineID() const
		{
			return 0xEC2C03A3;
		}

		bool PlayerSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
		{
			return MemUtils::CallClassMethod<bool, PlayerSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07ABEC0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
		}

		bool PlayerSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData)
		{
			return MemUtils::CallClassMethod<bool, PlayerSM*, uint32_t, float, uint32_t, void*>(0x07AB790, this, SimTime, FrameTime, TransID, TransData);
		}

		void PlayerSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
		{
			// call InterruptableStateMachine directly
			MemUtils::CallClassMethod<void, StateMachine*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x4ABA80, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
		}

		void PlayerSM::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
		{
			// call InterruptableStateMachine directly
			MemUtils::CallClassMethod<void, StateMachine*, uint32_t, float, StateMachine*>(0x4ABAC0, this, SimTime, FrameTime, Child);
		}

		void PlayerSM::RemoveChild(StateMachine* ChildMachine)
		{
			EARS::StateMachineSys::StateMachine::RemoveChild(ChildMachine);
		}

		void PlayerSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
		{
			// call AnimateStateMachine directly
			MemUtils::CallClassMethod<void, StateMachine*, uint32_t, float, uint32_t>(0x4AC7E0, this, SimTime, FrameTime, TableID);
		}

		void PlayerSM::InitialiseChild(StateMachine* ChildMachine)
		{
			if (ChildMachine->GetStateMachineID() == 0x29CC4DD4)
			{
				// TODO: Set Grabbee in PlayerDebugFlySM
			}

			EARS::StateMachineSys::StateMachine::InitialiseChild(ChildMachine);
		}
	}
}
