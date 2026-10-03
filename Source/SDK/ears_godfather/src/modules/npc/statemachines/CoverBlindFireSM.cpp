#include "CoverBlindFireSM.h"

namespace EARS::Modules
{
	CoverBlindFireSM::CoverBlindFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	CoverBlindFireSM::~CoverBlindFireSM()
	{
		MemUtils::CallClassMethod<void, CoverBlindFireSM*>(0x04DAF90, this);
	}

	bool CoverBlindFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CoverBlindFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07658C0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CoverBlindFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CoverBlindFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0765A10, this, SimTime, FrameTime, TransID, TransData);
	}

	void CoverBlindFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, CoverBlindFireSM*, EARS::StateMachineSys::StateMachine*>(0x0765520, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* CoverBlindFireSM::S_CoverBlindFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CoverBlindFireSM(InID, InSMParams);
	}
}
