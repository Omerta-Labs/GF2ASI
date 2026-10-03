#include "ContinualBurstFireSM.h"

namespace EARS::Modules
{
	ContinualBurstFireSM::ContinualBurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076BE70, this, TableID, SMParams);
	}

	ContinualBurstFireSM::~ContinualBurstFireSM()
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*>(0x04DAF90, this);
	}

	bool ContinualBurstFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ContinualBurstFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076C6A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ContinualBurstFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ContinualBurstFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076BF40, this, SimTime, FrameTime, TransID, TransData);
	}

	void ContinualBurstFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*, EARS::StateMachineSys::StateMachine*>(0x076BF80, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ContinualBurstFireSM::S_ContinualBurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ContinualBurstFireSM(InID, InSMParams);
	}
}
