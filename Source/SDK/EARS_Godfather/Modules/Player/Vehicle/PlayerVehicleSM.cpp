#include "PlayerVehicleSM.h"

namespace EARS::Modules
{
	PlayerVehicleSM::PlayerVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerVehicleSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07D3BD0, this, TableID, SMParams);
	}

	PlayerVehicleSM::~PlayerVehicleSM()
	{
		MemUtils::CallClassMethod<void, PlayerVehicleSM*>(0x07D54D0, this);
	}

	bool PlayerVehicleSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerVehicleSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07D5590, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerVehicleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerVehicleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07D4C20, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerVehicleSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerVehicleSM*, EARS::StateMachineSys::StateMachine*>(0x07D37D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerVehicleSM::S_PlayerVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerVehicleSM(InID, InSMParams);
	}
}
