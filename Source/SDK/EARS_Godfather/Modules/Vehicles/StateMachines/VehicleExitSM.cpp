#include "VehicleExitSM.h"

namespace EARS::Modules
{
	VehicleExitSM::VehicleExitSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, VehicleExitSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0884E40, this, TableID, SMParams);
	}

	VehicleExitSM::~VehicleExitSM()
	{
		MemUtils::CallClassMethod<void, VehicleExitSM*>(0x0886620, this);
	}

	bool VehicleExitSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, VehicleExitSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0886700, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool VehicleExitSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, VehicleExitSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0885CC0, this, SimTime, FrameTime, TransID, TransData);
	}

	int VehicleExitSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, VehicleExitSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* VehicleExitSM::S_VehicleExitSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleExitSM(InID, InSMParams);
	}
}
