#include "NPCReviveSM.h"

namespace EARS::Modules
{
	NPCReviveSM::NPCReviveSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCReviveSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0748E10, this, TableID, SMParams);
	}

	NPCReviveSM::~NPCReviveSM()
	{
		MemUtils::CallClassMethod<void, NPCReviveSM*>(0x0749240, this);
	}

	bool NPCReviveSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCReviveSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0749660, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCReviveSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCReviveSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0748EB0, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCReviveSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCReviveSM*, EARS::StateMachineSys::StateMachine*>(0x07490F0, this, &ChildMachine);
	}

	int NPCReviveSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCReviveSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCReviveSM::S_NPCReviveSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCReviveSM(InID, InSMParams);
	}
}
