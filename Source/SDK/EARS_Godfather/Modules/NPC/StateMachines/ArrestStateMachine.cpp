#include "ArrestStateMachine.h"

namespace EARS::Modules
{
	ArrestStateMachine::ArrestStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ArrestStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0766540, this, TableID, SMParams);
	}

	ArrestStateMachine::~ArrestStateMachine()
	{
		MemUtils::CallClassMethod<void, ArrestStateMachine*>(0x0766C00, this);
	}

	bool ArrestStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ArrestStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0766FA0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ArrestStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ArrestStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0766630, this, SimTime, FrameTime, TransID, TransData);
	}

	void ArrestStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ArrestStateMachine*, EARS::StateMachineSys::StateMachine*>(0x0766AB0, this, &ChildMachine);
	}

	int ArrestStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, ArrestStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* ArrestStateMachine::S_ArrestStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ArrestStateMachine(InID, InSMParams);
	}
}
