#include "CoverPeekFireSM.h"

namespace EARS::Modules
{
	CoverPeekFireSM::CoverPeekFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	CoverPeekFireSM::~CoverPeekFireSM()
	{
		MemUtils::CallClassMethod<void, CoverPeekFireSM*>(0x04DAF90, this);
	}

	bool CoverPeekFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CoverPeekFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0766000, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CoverPeekFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CoverPeekFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07660B0, this, SimTime, FrameTime, TransID, TransData);
	}

	void CoverPeekFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, CoverPeekFireSM*, EARS::StateMachineSys::StateMachine*>(0x0765C30, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* CoverPeekFireSM::S_CoverPeekFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CoverPeekFireSM(InID, InSMParams);
	}
}
