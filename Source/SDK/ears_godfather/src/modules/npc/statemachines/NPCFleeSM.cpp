#include "NPCFleeSM.h"

namespace EARS::Modules
{
	NPCFleeSM::NPCFleeSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCFleeSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07603D0, this, TableID, SMParams);
	}

	NPCFleeSM::~NPCFleeSM()
	{
		MemUtils::CallClassMethod<void, NPCFleeSM*>(0x07604A0, this);
	}

	bool NPCFleeSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCFleeSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07617D0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCFleeSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCFleeSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07614F0, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCFleeSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCFleeSM*, EARS::StateMachineSys::StateMachine*>(0x0760E00, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCFleeSM::S_NPCFleeSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCFleeSM(InID, InSMParams);
	}
}
