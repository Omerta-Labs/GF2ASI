#include "PlayerSlamSM.h"

namespace EARS::Modules
{
	PlayerSlamSM::PlayerSlamSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerSlamSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C3F10, this, TableID, SMParams);
	}

	PlayerSlamSM::~PlayerSlamSM()
	{
		MemUtils::CallClassMethod<void, PlayerSlamSM*>(0x07C3FC0, this);
	}

	bool PlayerSlamSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerSlamSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C5270, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerSlamSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerSlamSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07C4E50, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerSlamSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerSlamSM*, EARS::StateMachineSys::StateMachine*>(0x07C43B0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerSlamSM::S_PlayerSlamSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerSlamSM(InID, InSMParams);
	}
}
