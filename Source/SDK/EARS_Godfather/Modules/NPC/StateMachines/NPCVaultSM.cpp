#include "NPCVaultSM.h"

namespace EARS::Modules
{
	NPCVaultSM::NPCVaultSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCVaultSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x074B7C0, this, TableID, SMParams);
	}

	NPCVaultSM::~NPCVaultSM()
	{
		MemUtils::CallClassMethod<void, NPCVaultSM*>(0x04DAF90, this);
	}

	bool NPCVaultSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCVaultSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x074BE10, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCVaultSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCVaultSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x074B610, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCVaultSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCVaultSM*, EARS::StateMachineSys::StateMachine*>(0x074BAB0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCVaultSM::S_NPCVaultSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCVaultSM(InID, InSMParams);
	}
}
