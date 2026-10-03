#include "NPCPlantCarBombSM.h"

namespace EARS::Modules
{
	NPCPlantCarBombSM::NPCPlantCarBombSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	NPCPlantCarBombSM::~NPCPlantCarBombSM()
	{
		MemUtils::CallClassMethod<void, NPCPlantCarBombSM*>(0x076A220, this);
	}

	bool NPCPlantCarBombSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCPlantCarBombSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076AC00, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCPlantCarBombSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCPlantCarBombSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076A630, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCPlantCarBombSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCPlantCarBombSM*, EARS::StateMachineSys::StateMachine*>(0x076AA30, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* NPCPlantCarBombSM::S_NPCPlantCarBombSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCPlantCarBombSM(InID, InSMParams);
	}
}
