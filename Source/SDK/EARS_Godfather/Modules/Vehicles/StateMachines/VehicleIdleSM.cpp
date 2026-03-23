#include "VehicleIdleSM.h"

namespace EARS::Modules
{
	VehicleIdleSM::VehicleIdleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, VehicleIdleSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x086B060, this, TableID, SMParams);
	}

	VehicleIdleSM::~VehicleIdleSM()
	{
		MemUtils::CallClassMethod<void, VehicleIdleSM*>(0x04AC1E0, this);
	}

	bool VehicleIdleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleIdleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x086B010, this, SimTime, FrameTime, TransID, TransData);
	}

	void VehicleIdleSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, VehicleIdleSM*, uint32_t, float, uint32_t>(0x086B0E0, this, SimTime, FrameTime, TableID);
	}

	int VehicleIdleSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, VehicleIdleSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* VehicleIdleSM::S_VehicleIdleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleIdleSM(InID, InSMParams);
	}
}
