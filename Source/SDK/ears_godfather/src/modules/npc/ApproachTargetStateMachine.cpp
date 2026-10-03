#include "ApproachTargetStateMachine.h"

namespace EARS::Modules
{
	ApproachTargetStateMachine::ApproachTargetStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
	}

	ApproachTargetStateMachine::~ApproachTargetStateMachine()
	{
		MemUtils::CallClassMethod<void, ApproachTargetStateMachine*>(0x04DAF90, this);
	}

	bool ApproachTargetStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ApproachTargetStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07419A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ApproachTargetStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ApproachTargetStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0741620, this, SimTime, FrameTime, TransID, TransData);
	}

	void ApproachTargetStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ApproachTargetStateMachine*, EARS::StateMachineSys::StateMachine*>(0x0741B40, this, &ChildMachine);
	}

	int ApproachTargetStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, ApproachTargetStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* ApproachTargetStateMachine::S_ApproachTargetStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ApproachTargetStateMachine(InID, InSMParams);
	}
}
