#include "CommonNPCSM.h"

namespace EARS::Modules
{
	CommonNPCSM::CommonNPCSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, CommonNPCSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x073ED00, this, TableID, SMParams);
	}

	CommonNPCSM::~CommonNPCSM()
	{
		MemUtils::CallClassMethod<void, CommonNPCSM*>(0x073F2E0, this);
	}

	bool CommonNPCSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CommonNPCSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x073F590, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CommonNPCSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CommonNPCSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x073F210, this, SimTime, FrameTime, TransID, TransData);
	}

	int CommonNPCSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, CommonNPCSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* CommonNPCSM::S_CommonNPCSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CommonNPCSM(InID, InSMParams);
	}
}
