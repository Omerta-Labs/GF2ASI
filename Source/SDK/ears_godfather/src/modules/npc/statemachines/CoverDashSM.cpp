#include "CoverDashSM.h"

namespace EARS::Modules
{
	CoverDashSM::CoverDashSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, CoverDashSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07673D0, this, TableID, SMParams);
	}

	CoverDashSM::~CoverDashSM()
	{
		MemUtils::CallClassMethod<void, CoverDashSM*>(0x04DAF90, this);
	}

	bool CoverDashSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CoverDashSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0767EA0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CoverDashSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CoverDashSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0767AC0, this, SimTime, FrameTime, TransID, TransData);
	}

	void CoverDashSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, CoverDashSM*, EARS::StateMachineSys::StateMachine*>(0x0768120, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* CoverDashSM::S_CoverDashSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CoverDashSM(InID, InSMParams);
	}
}
