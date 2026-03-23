#include "NPCInformSM.h"

namespace EARS::Modules
{
	NPCInformSM::NPCInformSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCInformSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07473C0, this, TableID, SMParams);
	}

	NPCInformSM::~NPCInformSM()
	{
		MemUtils::CallClassMethod<void, NPCInformSM*>(0x04DAF90, this);
	}

	bool NPCInformSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCInformSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0747910, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCInformSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCInformSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0747470, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCInformSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCInformSM*, EARS::StateMachineSys::StateMachine*>(0x0747560, this, &ChildMachine);
	}

	int NPCInformSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCInformSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCInformSM::S_NPCInformSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCInformSM(InID, InSMParams);
	}
}
