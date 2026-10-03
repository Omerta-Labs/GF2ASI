#include "MobCarApproachVenueSM.h"

namespace EARS::Modules
{
	MobCarApproachVenueSM::MobCarApproachVenueSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0882A00, this, TableID, SMParams);
	}

	MobCarApproachVenueSM::~MobCarApproachVenueSM()
	{
	}

	EARS::StateMachineSys::StateMachine* MobCarApproachVenueSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, MobCarApproachVenueSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x067F580, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void MobCarApproachVenueSM::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, uint32_t, float, EARS::StateMachineSys::StateMachine*>(0x0680E30, this, SimTime, FrameTime, Child);
	}

	void MobCarApproachVenueSM::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, EARS::StateMachineSys::StateMachine*>(0x07EE070, this, ChildMachine);
	}

	void MobCarApproachVenueSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, MobCarApproachVenueSM*, uint32_t, float, uint32_t>(0x0407F00, this, SimTime, FrameTime, TableID);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* MobCarApproachVenueSM::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, MobCarApproachVenueSM*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0407F10, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* MobCarApproachVenueSM::S_MobCarApproachVenueSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new MobCarApproachVenueSM(InID, InSMParams);
	}
}
