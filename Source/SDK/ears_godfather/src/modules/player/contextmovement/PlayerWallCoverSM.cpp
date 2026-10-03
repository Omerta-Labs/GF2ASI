#include "PlayerWallCoverSM.h"

namespace EARS::Modules
{
	PlayerWallCoverSM::PlayerWallCoverSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerWallCoverSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C7AC0, this, TableID, SMParams);
	}

	PlayerWallCoverSM::~PlayerWallCoverSM()
	{
		MemUtils::CallClassMethod<void, PlayerWallCoverSM*>(0x07C8D80, this);
	}

	bool PlayerWallCoverSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerWallCoverSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C9E80, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerWallCoverSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerWallCoverSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07CA5A0, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerWallCoverSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerWallCoverSM*, EARS::StateMachineSys::StateMachine*>(0x07C9C40, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerWallCoverSM::S_PlayerWallCoverSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerWallCoverSM(InID, InSMParams);
	}
}
