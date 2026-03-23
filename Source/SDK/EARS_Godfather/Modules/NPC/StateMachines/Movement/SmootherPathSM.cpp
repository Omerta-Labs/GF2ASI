#include "SmootherPathSM.h"

namespace EARS::Modules
{
	SmootherPathSM::SmootherPathSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, SmootherPathSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07650B0, this, TableID, SMParams);
	}

	SmootherPathSM::~SmootherPathSM()
	{
		MemUtils::CallClassMethod<void, SmootherPathSM*>(0x04DAF90, this);
	}

	bool SmootherPathSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, SmootherPathSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0764F20, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool SmootherPathSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, SmootherPathSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x04AC640, this, SimTime, FrameTime, TransID, TransData);
	}

	void SmootherPathSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, SmootherPathSM*, EARS::StateMachineSys::StateMachine*>(0x07651E0, this, &ChildMachine);
	}

	int SmootherPathSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, SmootherPathSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* SmootherPathSM::S_SmootherPathSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new SmootherPathSM(InID, InSMParams);
	}
}
