#include "MustFollowPathStateMachine.h"

namespace EARS::Modules
{
	MustFollowPathStateMachine::MustFollowPathStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, MustFollowPathStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x075A2C0, this, TableID, SMParams);
	}

	MustFollowPathStateMachine::~MustFollowPathStateMachine()
	{
		MemUtils::CallClassMethod<void, MustFollowPathStateMachine*>(0x075A550, this);
	}

	bool MustFollowPathStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, MustFollowPathStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x075B150, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool MustFollowPathStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, MustFollowPathStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x075A600, this, SimTime, FrameTime, TransID, TransData);
	}

	void MustFollowPathStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, MustFollowPathStateMachine*, EARS::StateMachineSys::StateMachine*>(0x075AA10, this, &ChildMachine);
	}

	int MustFollowPathStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, MustFollowPathStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* MustFollowPathStateMachine::S_MustFollowPathStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new MustFollowPathStateMachine(InID, InSMParams);
	}
}
