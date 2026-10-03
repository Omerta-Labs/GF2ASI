#include "ReloadGunSM.h"

namespace EARS::Modules
{
	ReloadGunSM::ReloadGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	ReloadGunSM::~ReloadGunSM()
	{
		MemUtils::CallClassMethod<void, ReloadGunSM*>(0x0716310, this);
	}

	bool ReloadGunSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ReloadGunSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07687F0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ReloadGunSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ReloadGunSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07684D0, this, SimTime, FrameTime, TransID, TransData);
	}

	void ReloadGunSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ReloadGunSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ReloadGunSM::S_ReloadGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ReloadGunSM(InID, InSMParams);
	}
}
