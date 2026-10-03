#include "PoliceCarPatrolSM.h"

namespace EARS::Modules
{
	PoliceCarPatrolSM::PoliceCarPatrolSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0882A00, this, TableID, SMParams);
	}

	PoliceCarPatrolSM::~PoliceCarPatrolSM()
	{
	}

	EARS::StateMachineSys::StateMachine* PoliceCarPatrolSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, PoliceCarPatrolSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x067F580, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void PoliceCarPatrolSM::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, uint32_t, float, EARS::StateMachineSys::StateMachine*>(0x0680E30, this, SimTime, FrameTime, Child);
	}

	void PoliceCarPatrolSM::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, EARS::StateMachineSys::StateMachine*>(0x07EE070, this, ChildMachine);
	}

	void PoliceCarPatrolSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, PoliceCarPatrolSM*, uint32_t, float, uint32_t>(0x0407F00, this, SimTime, FrameTime, TableID);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* PoliceCarPatrolSM::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, PoliceCarPatrolSM*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0407F10, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* PoliceCarPatrolSM::S_PoliceCarPatrolSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PoliceCarPatrolSM(InID, InSMParams);
	}
}
