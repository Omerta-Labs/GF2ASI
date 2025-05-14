#include "PlayerMasterSM.h"

// Addons
#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDebug.h"

EARS::Modules::PlayerMasterSM::PlayerMasterSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
	: EARS::Modules::PlayerSM::PlayerSM(TableID, SMParams)
{
	EARS::Modules::Player* OwnPlayer = GetPlayer();
	OwnPlayer->SetSecondaryAnimSlots(2);
}

EARS::Modules::PlayerMasterSM::~PlayerMasterSM()
{
	// should be handled in base class
}

uint32_t EARS::Modules::PlayerMasterSM::GetStateMachineID() const
{
	return 0xB08AE1F6;
}

bool EARS::Modules::PlayerMasterSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
{
	if (MessageID == 49)
	{
		// TODO: In debug mode we check grab type
		return true;
	}

	// call parent class
	return EARS::Modules::PlayerSM::HandleStateMessage(SimTime, FrameTime, CurFlags, MessageID, MsgData);
}

bool EARS::Modules::PlayerMasterSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData)
{
	if (TransID == 17)
	{
		// should we enter fly mode
		EARS::Modules::PlayerDebugOptions& DebugOptions = *EARS::Modules::PlayerDebugOptions::GetInstance();
		return DebugOptions.IsInDebugFly();
	}

	// call parent class
	return EARS::Modules::PlayerSM::CheckTransition(SimTime, FrameTime, TransID, TransData);
}

void EARS::Modules::PlayerMasterSM::InitialiseChild(StateMachine* ChildMachine)
{
	if (ChildMachine->GetStateMachineID() == 0x29CC4DD4)
	{
		// TODO: Set Grabbee in PlayerDebugFlySM
	}

	EARS::StateMachineSys::StateMachine::InitialiseChild(ChildMachine);
}
