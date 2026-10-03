#include "npcguncombatsm.h"

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

	DrawGunSM::DrawGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	DrawGunSM::~DrawGunSM()
	{
		MemUtils::CallClassMethod<void, DrawGunSM*>(0x0768C20, this);
	}

	bool DrawGunSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, DrawGunSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0768D70, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool DrawGunSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, DrawGunSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0768590, this, SimTime, FrameTime, TransID, TransData);
	}

	void DrawGunSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, DrawGunSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* DrawGunSM::S_DrawGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new DrawGunSM(InID, InSMParams);
	}

	HolsterGunSM::HolsterGunSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	HolsterGunSM::~HolsterGunSM()
	{
		MemUtils::CallClassMethod<void, HolsterGunSM*>(0x04DAF90, this);
	}

	bool HolsterGunSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, HolsterGunSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0768A70, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool HolsterGunSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, HolsterGunSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0768650, this, SimTime, FrameTime, TransID, TransData);
	}

	void HolsterGunSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, HolsterGunSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* HolsterGunSM::S_HolsterGunSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new HolsterGunSM(InID, InSMParams);
	}
}
