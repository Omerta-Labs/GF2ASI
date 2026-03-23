#include "PainStateMachine.h"

namespace EARS::Modules
{
	PainStateMachine::PainStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PainStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076E970, this, TableID, SMParams);
	}

	PainStateMachine::~PainStateMachine()
	{
		MemUtils::CallClassMethod<void, PainStateMachine*>(0x076EA40, this);
	}

	bool PainStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PainStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07711A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PainStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PainStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0771720, this, SimTime, FrameTime, TransID, TransData);
	}

	void PainStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PainStateMachine*, EARS::StateMachineSys::StateMachine*>(0x076EED0, this, &ChildMachine);
	}

	int PainStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, PainStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* PainStateMachine::S_PainStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PainStateMachine(InID, InSMParams);
	}
}
