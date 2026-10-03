#include "CoverPeekSM.h"

namespace EARS::Modules
{
	CoverPeekSM::CoverPeekSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	CoverPeekSM::~CoverPeekSM()
	{
		MemUtils::CallClassMethod<void, CoverPeekSM*>(0x04DAF90, this);
	}

	bool CoverPeekSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CoverPeekSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0768360, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CoverPeekSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CoverPeekSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0767620, this, SimTime, FrameTime, TransID, TransData);
	}

	void CoverPeekSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, CoverPeekSM*, EARS::StateMachineSys::StateMachine*>(0x07675B0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* CoverPeekSM::S_CoverPeekSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CoverPeekSM(InID, InSMParams);
	}
}
