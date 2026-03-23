#include "NPCStrafeSM.h"

namespace EARS::Modules
{
	NPCStrafeSM::NPCStrafeSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCStrafeSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x075FAA0, this, TableID, SMParams);
	}

	NPCStrafeSM::~NPCStrafeSM()
	{
		MemUtils::CallClassMethod<void, NPCStrafeSM*>(0x04DAF90, this);
	}

	bool NPCStrafeSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCStrafeSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x075F9E0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCStrafeSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCStrafeSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x075FA40, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCStrafeSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCStrafeSM*, EARS::StateMachineSys::StateMachine*>(0x075FD60, this, &ChildMachine);
	}

	int NPCStrafeSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCStrafeSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCStrafeSM::S_NPCStrafeSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCStrafeSM(InID, InSMParams);
	}
}
