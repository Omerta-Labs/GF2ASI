#include "PlayerGrappleSM.h"

namespace EARS::Modules
{
	PlayerGrappleSM::PlayerGrappleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerGrappleSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07B1800, this, TableID, SMParams);
	}

	PlayerGrappleSM::~PlayerGrappleSM()
	{
		MemUtils::CallClassMethod<void, PlayerGrappleSM*>(0x07B0030, this);
	}

	bool PlayerGrappleSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGrappleSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07B1A50, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerGrappleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGrappleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07B3F30, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerGrappleSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerGrappleSM*, EARS::StateMachineSys::StateMachine*>(0x07AF540, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerGrappleSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, PlayerGrappleSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x07B03F0, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	EARS::StateMachineSys::StateMachine* PlayerGrappleSM::S_PlayerGrappleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerGrappleSM(InID, InSMParams);
	}
}
