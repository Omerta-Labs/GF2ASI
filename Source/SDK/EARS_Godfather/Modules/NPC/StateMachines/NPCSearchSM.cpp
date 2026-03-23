#include "NPCSearchSM.h"

namespace EARS::Modules
{
	NPCSearchSM::NPCSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCSearchSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x074A840, this, TableID, SMParams);
	}

	NPCSearchSM::~NPCSearchSM()
	{
		MemUtils::CallClassMethod<void, NPCSearchSM*>(0x04DAF90, this);
	}

	bool NPCSearchSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCSearchSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x074ABD0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCSearchSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCSearchSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x074AB00, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCSearchSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCSearchSM*, EARS::StateMachineSys::StateMachine*>(0x074AD10, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCSearchSM::S_NPCSearchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCSearchSM(InID, InSMParams);
	}
}
