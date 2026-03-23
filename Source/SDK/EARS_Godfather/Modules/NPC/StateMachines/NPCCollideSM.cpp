#include "NPCCollideSM.h"

namespace EARS::Modules
{
	NPCCollideSM::NPCCollideSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCCollideSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x073D5D0, this, TableID, SMParams);
	}

	NPCCollideSM::~NPCCollideSM()
	{
		MemUtils::CallClassMethod<void, NPCCollideSM*>(0x04DAF90, this);
	}

	bool NPCCollideSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCCollideSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x073D4B0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCCollideSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCCollideSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x073D7F0, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCCollideSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCCollideSM*, EARS::StateMachineSys::StateMachine*>(0x073D830, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCCollideSM::S_NPCCollideSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCCollideSM(InID, InSMParams);
	}
}
