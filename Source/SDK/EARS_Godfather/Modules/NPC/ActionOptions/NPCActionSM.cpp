#include "NPCActionSM.h"

namespace EARS::Modules
{
	NPCActionSM::NPCActionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCActionSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x077F520, this, TableID, SMParams);
	}

	NPCActionSM::~NPCActionSM()
	{
		MemUtils::CallClassMethod<void, NPCActionSM*>(0x077EF20, this);
	}

	bool NPCActionSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCActionSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0780EA0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCActionSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCActionSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0780B50, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCActionSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCActionSM*, EARS::StateMachineSys::StateMachine*>(0x0781980, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCActionSM::S_NPCActionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCActionSM(InID, InSMParams);
	}
}
