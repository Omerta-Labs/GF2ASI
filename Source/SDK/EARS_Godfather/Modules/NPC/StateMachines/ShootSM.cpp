#include "ShootSM.h"

namespace EARS::Modules
{
	ShootSM::ShootSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ShootSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076D020, this, TableID, SMParams);
	}

	ShootSM::~ShootSM()
	{
		MemUtils::CallClassMethod<void, ShootSM*>(0x072CB10, this);
	}

	bool ShootSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ShootSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076BC00, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ShootSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ShootSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076BCD0, this, SimTime, FrameTime, TransID, TransData);
	}

	void ShootSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ShootSM*, EARS::StateMachineSys::StateMachine*>(0x076C2F0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ShootSM::S_ShootSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ShootSM(InID, InSMParams);
	}
}
