#include "NPCBackupSM.h"

namespace EARS::Modules
{
	NPCBackupSM::NPCBackupSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCBackupSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0744650, this, TableID, SMParams);
	}

	NPCBackupSM::~NPCBackupSM()
	{
		MemUtils::CallClassMethod<void, NPCBackupSM*>(0x075CC50, this);
	}

	bool NPCBackupSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCBackupSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0744AC0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCBackupSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCBackupSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0744DA0, this, SimTime, FrameTime, TransID, TransData);
	}

	int NPCBackupSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCBackupSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCBackupSM::S_NPCBackupSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCBackupSM(InID, InSMParams);
	}
}
