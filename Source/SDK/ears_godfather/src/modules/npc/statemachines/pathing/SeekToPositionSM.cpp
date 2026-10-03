#include "SeekToPositionSM.h"

namespace EARS::Modules
{
	SeekToPositionSM::SeekToPositionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, SeekToPositionSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x075E510, this, TableID, SMParams);
	}

	SeekToPositionSM::~SeekToPositionSM()
	{
		MemUtils::CallClassMethod<void, SeekToPositionSM*>(0x075CC50, this);
	}

	bool SeekToPositionSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, SeekToPositionSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x075F8F0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool SeekToPositionSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, SeekToPositionSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x075F270, this, SimTime, FrameTime, TransID, TransData);
	}

	void SeekToPositionSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, SeekToPositionSM*, EARS::StateMachineSys::StateMachine*>(0x075EBE0, this, &ChildMachine);
	}

	int SeekToPositionSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, SeekToPositionSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* SeekToPositionSM::S_SeekToPositionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new SeekToPositionSM(InID, InSMParams);
	}
}
