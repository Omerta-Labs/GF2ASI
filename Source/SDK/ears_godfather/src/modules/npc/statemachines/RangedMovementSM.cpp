#include "RangedMovementSM.h"

namespace EARS::Modules
{
	RangedMovementSM::RangedMovementSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, RangedMovementSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0750940, this, TableID, SMParams);
	}

	RangedMovementSM::~RangedMovementSM()
	{
		MemUtils::CallClassMethod<void, RangedMovementSM*>(0x0750B50, this);
	}

	bool RangedMovementSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, RangedMovementSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0751560, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool RangedMovementSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, RangedMovementSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07500F0, this, SimTime, FrameTime, TransID, TransData);
	}

	void RangedMovementSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, RangedMovementSM*, EARS::StateMachineSys::StateMachine*>(0x0750630, this, &ChildMachine);
	}

	int RangedMovementSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, RangedMovementSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* RangedMovementSM::S_RangedMovementSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new RangedMovementSM(InID, InSMParams);
	}
}
