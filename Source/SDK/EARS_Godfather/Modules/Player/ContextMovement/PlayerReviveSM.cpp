#include "PlayerReviveSM.h"

namespace EARS::Modules
{
	PlayerReviveSM::PlayerReviveSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerReviveSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C37E0, this, TableID, SMParams);
	}

	PlayerReviveSM::~PlayerReviveSM()
	{
		MemUtils::CallClassMethod<void, PlayerReviveSM*>(0x07AB130, this);
	}

	bool PlayerReviveSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerReviveSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C3C90, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerReviveSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerReviveSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07C38A0, this, SimTime, FrameTime, TransID, TransData);
	}

	void PlayerReviveSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, PlayerReviveSM*, EARS::StateMachineSys::StateMachine*>(0x07C3A70, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* PlayerReviveSM::S_PlayerReviveSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerReviveSM(InID, InSMParams);
	}
}
