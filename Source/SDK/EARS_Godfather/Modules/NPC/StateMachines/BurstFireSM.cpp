#include "BurstFireSM.h"

namespace EARS::Modules
{
	BurstFireSM::BurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	BurstFireSM::~BurstFireSM()
	{
		MemUtils::CallClassMethod<void, BurstFireSM*>(0x04DAF90, this);
	}

	bool BurstFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, BurstFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076CAC0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool BurstFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, BurstFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076CF10, this, SimTime, FrameTime, TransID, TransData);
	}

	void BurstFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, BurstFireSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* BurstFireSM::S_BurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new BurstFireSM(InID, InSMParams);
	}
}
