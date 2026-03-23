#include "SpecialMotionSM.h"

namespace EARS::Modules
{
	SpecialMotionSM::SpecialMotionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, SpecialMotionSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0752130, this, TableID, SMParams);
	}

	SpecialMotionSM::~SpecialMotionSM()
	{
		MemUtils::CallClassMethod<void, SpecialMotionSM*>(0x07521F0, this);
	}

	bool SpecialMotionSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, SpecialMotionSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0752290, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	void SpecialMotionSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, SpecialMotionSM*, EARS::StateMachineSys::StateMachine*>(0x0752810, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* SpecialMotionSM::S_SpecialMotionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new SpecialMotionSM(InID, InSMParams);
	}
}
