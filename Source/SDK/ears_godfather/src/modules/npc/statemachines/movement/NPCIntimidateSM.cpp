#include "NPCIntimidateSM.h"

namespace EARS::Modules
{
	NPCIntimidateSM::NPCIntimidateSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
	}

	NPCIntimidateSM::~NPCIntimidateSM()
	{
		MemUtils::CallClassMethod<void, NPCIntimidateSM*>(0x0747C50, this);
	}

	bool NPCIntimidateSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCIntimidateSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0747E90, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCIntimidateSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCIntimidateSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0747CB0, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCIntimidateSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCIntimidateSM*, EARS::StateMachineSys::StateMachine*>(0x0748090, this, &ChildMachine);
	}

	int NPCIntimidateSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCIntimidateSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCIntimidateSM::S_NPCIntimidateSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCIntimidateSM(InID, InSMParams);
	}
}
