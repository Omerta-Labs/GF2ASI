#include "GrabReactStateMachine.h"

namespace EARS::Modules
{
	GrabReactStateMachine::GrabReactStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, GrabReactStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x080C580, this, TableID, SMParams);
	}

	GrabReactStateMachine::~GrabReactStateMachine()
	{
		MemUtils::CallClassMethod<void, GrabReactStateMachine*>(0x080C9C0, this);
	}

	bool GrabReactStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, GrabReactStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x080CD30, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool GrabReactStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, GrabReactStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x080C600, this, SimTime, FrameTime, TransID, TransData);
	}

	int GrabReactStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, GrabReactStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* GrabReactStateMachine::S_GrabReactStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new GrabReactStateMachine(InID, InSMParams);
	}
}
