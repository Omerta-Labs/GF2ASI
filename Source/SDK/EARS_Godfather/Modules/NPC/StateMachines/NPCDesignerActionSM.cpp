#include "NPCDesignerActionSM.h"

namespace EARS::Modules
{
	NPCDesignerActionSM::NPCDesignerActionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCDesignerActionSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x073D9F0, this, TableID, SMParams);
	}

	NPCDesignerActionSM::~NPCDesignerActionSM()
	{
		MemUtils::CallClassMethod<void, NPCDesignerActionSM*>(0x04DAF90, this);
	}

	bool NPCDesignerActionSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCDesignerActionSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x073DCD0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCDesignerActionSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCDesignerActionSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x073DE80, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCDesignerActionSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCDesignerActionSM*, EARS::StateMachineSys::StateMachine*>(0x073DF20, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCDesignerActionSM::S_NPCDesignerActionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCDesignerActionSM(InID, InSMParams);
	}
}
