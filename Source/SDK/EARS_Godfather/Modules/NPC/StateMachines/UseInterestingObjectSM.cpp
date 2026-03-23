#include "UseInterestingObjectSM.h"

namespace EARS::Modules
{
	UseInterestingObjectSM::UseInterestingObjectSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, UseInterestingObjectSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x073FB40, this, TableID, SMParams);
	}

	UseInterestingObjectSM::~UseInterestingObjectSM()
	{
		MemUtils::CallClassMethod<void, UseInterestingObjectSM*>(0x073FCE0, this);
	}

	bool UseInterestingObjectSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, UseInterestingObjectSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0740AD0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool UseInterestingObjectSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, UseInterestingObjectSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07411E0, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* UseInterestingObjectSM::S_UseInterestingObjectSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new UseInterestingObjectSM(InID, InSMParams);
	}
}
