#include "VehicleDodgeStateMachine.h"

namespace EARS::Modules
{
	VehicleDodgeStateMachine::VehicleDodgeStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, VehicleDodgeStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07563E0, this, TableID, SMParams);
	}

	VehicleDodgeStateMachine::~VehicleDodgeStateMachine()
	{
		MemUtils::CallClassMethod<void, VehicleDodgeStateMachine*>(0x0756270, this);
	}

	bool VehicleDodgeStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, VehicleDodgeStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0756AA0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool VehicleDodgeStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleDodgeStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07562F0, this, SimTime, FrameTime, TransID, TransData);
	}

	int VehicleDodgeStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, VehicleDodgeStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* VehicleDodgeStateMachine::S_VehicleDodgeStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleDodgeStateMachine(InID, InSMParams);
	}
}
