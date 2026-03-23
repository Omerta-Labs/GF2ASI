#include "PlayerH2HSM.h"

namespace EARS::Modules
{
	PlayerH2HSM::PlayerH2HSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerH2HSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07BAEC0, this, TableID, SMParams);
	}

	PlayerH2HSM::~PlayerH2HSM()
	{
		MemUtils::CallClassMethod<void, PlayerH2HSM*>(0x07BA0D0, this);
	}

	bool PlayerH2HSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerH2HSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07BECE0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerH2HSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerH2HSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07BB010, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerH2HSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerH2HSM*, EARS::StateMachineSys::StateMachine*>(0x07BF1D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerH2HSM::S_PlayerH2HSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerH2HSM(InID, InSMParams);
	}
}
