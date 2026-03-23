#include "NPCGuardSM.h"

namespace EARS::Modules
{
	NPCGuardSM::NPCGuardSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCGuardSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0746BC0, this, TableID, SMParams);
	}

	NPCGuardSM::~NPCGuardSM()
	{
		MemUtils::CallClassMethod<void, NPCGuardSM*>(0x04DAF90, this);
	}

	bool NPCGuardSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCGuardSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0747010, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCGuardSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCGuardSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0747200, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCGuardSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCGuardSM*, EARS::StateMachineSys::StateMachine*>(0x07472F0, this, &ChildMachine);
	}

	int NPCGuardSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCGuardSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCGuardSM::S_NPCGuardSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCGuardSM(InID, InSMParams);
	}
}
