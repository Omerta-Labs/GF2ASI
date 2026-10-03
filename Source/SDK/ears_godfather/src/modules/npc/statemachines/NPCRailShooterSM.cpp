#include "NPCRailShooterSM.h"

namespace EARS::Modules
{
	NPCRailShooterSM::NPCRailShooterSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCRailShooterSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0776750, this, TableID, SMParams);
	}

	NPCRailShooterSM::~NPCRailShooterSM()
	{
		MemUtils::CallClassMethod<void, NPCRailShooterSM*>(0x07774B0, this);
	}

	bool NPCRailShooterSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCRailShooterSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07781A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCRailShooterSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCRailShooterSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0777A20, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCRailShooterSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCRailShooterSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCRailShooterSM::S_NPCRailShooterSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCRailShooterSM(InID, InSMParams);
	}
}
