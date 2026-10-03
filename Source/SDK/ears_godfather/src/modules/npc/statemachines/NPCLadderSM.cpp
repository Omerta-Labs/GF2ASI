#include "NPCLadderSM.h"

namespace EARS::Modules
{
	NPCLadderSM::NPCLadderSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCLadderSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0748570, this, TableID, SMParams);
	}

	NPCLadderSM::~NPCLadderSM()
	{
		MemUtils::CallClassMethod<void, NPCLadderSM*>(0x0751EC0, this);
	}

	bool NPCLadderSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCLadderSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07489B0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCLadderSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCLadderSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0748B00, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCLadderSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCLadderSM*, EARS::StateMachineSys::StateMachine*>(0x0748C90, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCLadderSM::S_NPCLadderSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCLadderSM(InID, InSMParams);
	}
}
