#include "VehicleChaseOffsetStateMachine.h"

namespace EARS::Modules
{
	VehicleChaseOffsetStateMachine::VehicleChaseOffsetStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x086ECC0, this, TableID, SMParams);
	}

	VehicleChaseOffsetStateMachine::~VehicleChaseOffsetStateMachine()
	{
	}

	EARS::StateMachineSys::StateMachine* VehicleChaseOffsetStateMachine::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, VehicleChaseOffsetStateMachine*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x067F580, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void VehicleChaseOffsetStateMachine::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, uint32_t, float, EARS::StateMachineSys::StateMachine*>(0x0680E30, this, SimTime, FrameTime, Child);
	}

	void VehicleChaseOffsetStateMachine::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, EARS::StateMachineSys::StateMachine*>(0x07EE070, this, ChildMachine);
	}

	void VehicleChaseOffsetStateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, VehicleChaseOffsetStateMachine*, uint32_t, float, uint32_t>(0x0407F00, this, SimTime, FrameTime, TableID);
	}

	const EARS::StateMachineSys::StateMachineSnapshot* VehicleChaseOffsetStateMachine::ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<const EARS::StateMachineSys::StateMachineSnapshot*, VehicleChaseOffsetStateMachine*, const EARS::StateMachineSys::StateMachineSnapshot*>(0x0407F10, this, pSnap);
	}

	EARS::StateMachineSys::StateMachine* VehicleChaseOffsetStateMachine::S_VehicleChaseOffsetStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleChaseOffsetStateMachine(InID, InSMParams);
	}
}
