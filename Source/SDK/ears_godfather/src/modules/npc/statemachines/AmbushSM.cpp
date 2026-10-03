#include "AmbushSM.h"

namespace EARS::Modules
{
	AmbushSM::AmbushSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	AmbushSM::~AmbushSM()
	{
		MemUtils::CallClassMethod<void, AmbushSM*>(0x04DAF90, this);
	}

	bool AmbushSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, AmbushSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x074A360, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool AmbushSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, AmbushSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0749D90, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* AmbushSM::S_AmbushSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new AmbushSM(InID, InSMParams);
	}
}
