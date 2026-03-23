#include "NPCWitnessIdleSM.h"

namespace EARS::Modules
{
	NPCWitnessIdleSM::NPCWitnessIdleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCWitnessIdleSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0776020, this, TableID, SMParams);
	}

	NPCWitnessIdleSM::~NPCWitnessIdleSM()
	{
		MemUtils::CallClassMethod<void, NPCWitnessIdleSM*>(0x04AC1E0, this);
	}

	bool NPCWitnessIdleSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCWitnessIdleSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0775FB0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCWitnessIdleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCWitnessIdleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0776240, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCWitnessIdleSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCWitnessIdleSM*, EARS::StateMachineSys::StateMachine*>(0x0776310, this, &ChildMachine);
	}

	int NPCWitnessIdleSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCWitnessIdleSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCWitnessIdleSM::S_NPCWitnessIdleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCWitnessIdleSM(InID, InSMParams);
	}
}
