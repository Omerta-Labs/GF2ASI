#include "CrewCommandStateMachine.h"

namespace EARS::Modules
{
	CrewCommandStateMachine::CrewCommandStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, CrewCommandStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x073B5B0, this, TableID, SMParams);
	}

	CrewCommandStateMachine::~CrewCommandStateMachine()
	{
		MemUtils::CallClassMethod<void, CrewCommandStateMachine*>(0x073B910, this);
	}

	bool CrewCommandStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CrewCommandStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x073BB30, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CrewCommandStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CrewCommandStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x073C3F0, this, SimTime, FrameTime, TransID, TransData);
	}

	void CrewCommandStateMachine::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, CrewCommandStateMachine*, EARS::StateMachineSys::StateMachine*>(0x073C750, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* CrewCommandStateMachine::S_CrewCommandStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CrewCommandStateMachine(InID, InSMParams);
	}
}
