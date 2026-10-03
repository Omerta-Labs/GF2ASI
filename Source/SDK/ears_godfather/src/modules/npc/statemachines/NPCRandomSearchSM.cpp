#include "NPCRandomSearchSM.h"

namespace EARS::Modules
{
	NPCRandomSearchSM::NPCRandomSearchSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCRandomSearchSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0749F60, this, TableID, SMParams);
	}

	NPCRandomSearchSM::~NPCRandomSearchSM()
	{
		MemUtils::CallClassMethod<void, NPCRandomSearchSM*>(0x04DAF90, this);
	}

	bool NPCRandomSearchSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCRandomSearchSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x074ABD0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCRandomSearchSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCRandomSearchSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x074AB00, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCRandomSearchSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCRandomSearchSM*, EARS::StateMachineSys::StateMachine*>(0x074AD10, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCRandomSearchSM::S_NPCRandomSearchSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCRandomSearchSM(InID, InSMParams);
	}
}
