#include "PlayerStrafeMovementSM.h"

namespace EARS::Modules
{
	PlayerStrafeMovementSM::PlayerStrafeMovementSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerStrafeMovementSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07D28F0, this, TableID, SMParams);
	}

	PlayerStrafeMovementSM::~PlayerStrafeMovementSM()
	{
		MemUtils::CallClassMethod<void, PlayerStrafeMovementSM*>(0x04DAF90, this);
	}

	bool PlayerStrafeMovementSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerStrafeMovementSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07D3390, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	EARS::StateMachineSys::StateMachine* PlayerStrafeMovementSM::S_PlayerStrafeMovementSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerStrafeMovementSM(InID, InSMParams);
	}
}
