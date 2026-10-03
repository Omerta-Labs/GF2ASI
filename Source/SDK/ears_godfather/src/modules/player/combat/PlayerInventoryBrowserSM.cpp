#include "PlayerInventoryBrowserSM.h"

namespace EARS::Modules
{
	PlayerInventoryBrowserSM::PlayerInventoryBrowserSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
	}

	PlayerInventoryBrowserSM::~PlayerInventoryBrowserSM()
	{
		MemUtils::CallClassMethod<void, PlayerInventoryBrowserSM*>(0x07AB650, this);
	}

	bool PlayerInventoryBrowserSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerInventoryBrowserSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07DEA20, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerInventoryBrowserSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerInventoryBrowserSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07DE880, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerInventoryBrowserSM::S_PlayerInventoryBrowserSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerInventoryBrowserSM(InID, InSMParams);
	}
}
