#include "PlayerGenericCSASM.h"

namespace EARS::Modules
{
	PlayerGenericCSASM::PlayerGenericCSASM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerGenericCSASM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07BFDF0, this, TableID, SMParams);
	}

	PlayerGenericCSASM::~PlayerGenericCSASM()
	{
		MemUtils::CallClassMethod<void, PlayerGenericCSASM*>(0x07BFF10, this);
	}

	bool PlayerGenericCSASM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGenericCSASM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07C01E0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool PlayerGenericCSASM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, PlayerGenericCSASM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07C0510, this, SimTime, FrameTime, TransID, TransData);
	}

	EARS::StateMachineSys::StateMachine* PlayerGenericCSASM::S_PlayerGenericCSASM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerGenericCSASM(InID, InSMParams);
	}
}
