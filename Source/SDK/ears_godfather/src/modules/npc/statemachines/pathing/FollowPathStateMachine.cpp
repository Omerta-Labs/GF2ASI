#include "FollowPathStateMachine.h"

namespace EARS::Modules
{
	FollowPathStateMachine::FollowPathStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, FollowPathStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07575A0, this, TableID, SMParams);
	}

	FollowPathStateMachine::~FollowPathStateMachine()
	{
	}

	bool FollowPathStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, FollowPathStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0716D00, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool FollowPathStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, FollowPathStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x071A520, this, SimTime, FrameTime, TransID, TransData);
	}

	void FollowPathStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, FollowPathStateMachine*, EARS::StateMachineSys::StateMachine*>(0x0717D70, this, &ChildMachine);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* FollowPathStateMachine::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, FollowPathStateMachine*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0719BA0, this, pSnap);
	}

	EARS::StateMachineSys::StateMachineSnapshot* FollowPathStateMachine::WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachineSnapshot*, FollowPathStateMachine*, EARS::StateMachineSys::StateMachineSnapshot*>(0x071A290, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* FollowPathStateMachine::S_FollowPathStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new FollowPathStateMachine(InID, InSMParams);
	}
}
