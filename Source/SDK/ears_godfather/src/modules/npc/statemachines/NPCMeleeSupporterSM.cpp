#include "NPCMeleeSupporterSM.h"

namespace EARS::Modules
{
	NPCMeleeSupporterSM::NPCMeleeSupporterSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCMeleeSupporterSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0769450, this, TableID, SMParams);
	}

	NPCMeleeSupporterSM::~NPCMeleeSupporterSM()
	{
		MemUtils::CallClassMethod<void, NPCMeleeSupporterSM*>(0x0768F10, this);
	}

	bool NPCMeleeSupporterSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCMeleeSupporterSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076A000, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCMeleeSupporterSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCMeleeSupporterSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0769B70, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCMeleeSupporterSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCMeleeSupporterSM*, EARS::StateMachineSys::StateMachine*>(0x0769D60, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCMeleeSupporterSM::S_NPCMeleeSupporterSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCMeleeSupporterSM(InID, InSMParams);
	}
}
