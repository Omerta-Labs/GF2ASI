#include "NPCDeathSM.h"

namespace EARS::Modules
{
	NPCDeathSM::NPCDeathSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCDeathSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076DCA0, this, TableID, SMParams);
	}

	NPCDeathSM::~NPCDeathSM()
	{
		MemUtils::CallClassMethod<void, NPCDeathSM*>(0x076DB60, this);
	}

	bool NPCDeathSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCDeathSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076DE50, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCDeathSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCDeathSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076D9E0, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCDeathSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCDeathSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCDeathSM::S_NPCDeathSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCDeathSM(InID, InSMParams);
	}
}
