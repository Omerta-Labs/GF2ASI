#include "PlayerHoldThrowSM.h"

namespace EARS::Modules
{
	PlayerHoldThrowSM::PlayerHoldThrowSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerHoldThrowSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C0B90, this, TableID, SMParams);
	}

	PlayerHoldThrowSM::~PlayerHoldThrowSM()
	{
		MemUtils::CallClassMethod<void, PlayerHoldThrowSM*>(0x07C0910, this);
	}

	bool PlayerHoldThrowSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerHoldThrowSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C16F0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerHoldThrowSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerHoldThrowSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07C1F00, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerHoldThrowSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerHoldThrowSM*, EARS::StateMachineSys::StateMachine*>(0x07C09F0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerHoldThrowSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, PlayerHoldThrowSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x07C11B0, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	EARS::StateMachineSys::StateMachine* PlayerHoldThrowSM::S_PlayerHoldThrowSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerHoldThrowSM(InID, InSMParams);
	}
}
