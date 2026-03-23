#include "PlayerVehicleHijackSM.h"

namespace EARS::Modules
{
	PlayerVehicleHijackSM::PlayerVehicleHijackSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerVehicleHijackSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C5BA0, this, TableID, SMParams);
	}

	PlayerVehicleHijackSM::~PlayerVehicleHijackSM()
	{
		MemUtils::CallClassMethod<void, PlayerVehicleHijackSM*>(0x07C5E50, this);
	}

	bool PlayerVehicleHijackSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerVehicleHijackSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C6160, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	void PlayerVehicleHijackSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerVehicleHijackSM*, EARS::StateMachineSys::StateMachine*>(0x07C5EE0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerVehicleHijackSM::S_PlayerVehicleHijackSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerVehicleHijackSM(InID, InSMParams);
	}
}
