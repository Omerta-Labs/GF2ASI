#include "NPCPunchSM.h"

namespace EARS::Modules
{
	NPCPunchSM::NPCPunchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCPunchSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076B0F0, this, TableID, SMParams);
	}

	NPCPunchSM::~NPCPunchSM()
	{
		MemUtils::CallClassMethod<void, NPCPunchSM*>(0x076AF90, this);
	}

	bool NPCPunchSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCPunchSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076B840, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCPunchSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCPunchSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076B040, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCPunchSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCPunchSM*, EARS::StateMachineSys::StateMachine*>(0x076B1B0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCPunchSM::S_NPCPunchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCPunchSM(InID, InSMParams);
	}
}
