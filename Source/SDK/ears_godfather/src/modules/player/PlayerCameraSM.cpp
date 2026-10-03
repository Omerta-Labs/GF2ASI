#include "PlayerCameraSM.h"

namespace EARS::Modules
{
	PlayerCameraSM::PlayerCameraSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerCameraSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07DABD0, this, TableID, SMParams);
	}

	PlayerCameraSM::~PlayerCameraSM()
	{
		MemUtils::CallClassMethod<void, PlayerCameraSM*>(0x07DACC0, this);
	}

	bool PlayerCameraSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerCameraSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DBDC0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	EARS::StateMachineSys::StateMachine* PlayerCameraSM::S_PlayerCameraSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerCameraSM(InID, InSMParams);
	}
}
