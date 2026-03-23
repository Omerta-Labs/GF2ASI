#include "NPCFollowSM.h"

namespace EARS::Modules
{
	NPCFollowSM::NPCFollowSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCFollowSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07454B0, this, TableID, SMParams);
	}

	NPCFollowSM::~NPCFollowSM()
	{
		MemUtils::CallClassMethod<void, NPCFollowSM*>(0x0745DD0, this);
	}

	bool NPCFollowSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCFollowSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07455C0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCFollowSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCFollowSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0745930, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCFollowSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCFollowSM*, EARS::StateMachineSys::StateMachine*>(0x0745BF0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCFollowSM::S_NPCFollowSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCFollowSM(InID, InSMParams);
	}
}
