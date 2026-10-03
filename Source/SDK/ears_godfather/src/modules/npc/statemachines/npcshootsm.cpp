#include "npcshootsm.h"

namespace EARS::Modules
{
	ShootSM::ShootSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ShootSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076D020, this, TableID, SMParams);
	}

	ShootSM::~ShootSM()
	{
		MemUtils::CallClassMethod<void, ShootSM*>(0x072CB10, this);
	}

	bool ShootSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ShootSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076BC00, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ShootSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ShootSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076BCD0, this, SimTime, FrameTime, TransID, TransData);
	}

	void ShootSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ShootSM*, EARS::StateMachineSys::StateMachine*>(0x076C2F0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ShootSM::S_ShootSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ShootSM(InID, InSMParams);
	}

	ContinualBurstFireSM::ContinualBurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x076BE70, this, TableID, SMParams);
	}

	ContinualBurstFireSM::~ContinualBurstFireSM()
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*>(0x04DAF90, this);
	}

	bool ContinualBurstFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, ContinualBurstFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076C6A0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool ContinualBurstFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, ContinualBurstFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076BF40, this, SimTime, FrameTime, TransID, TransData);
	}

	void ContinualBurstFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, ContinualBurstFireSM*, EARS::StateMachineSys::StateMachine*>(0x076BF80, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* ContinualBurstFireSM::S_ContinualBurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new ContinualBurstFireSM(InID, InSMParams);
	}

	BurstFireSM::BurstFireSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::SentientSM(TableID, SMParams)
	{
	}

	BurstFireSM::~BurstFireSM()
	{
		MemUtils::CallClassMethod<void, BurstFireSM*>(0x04DAF90, this);
	}

	bool BurstFireSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, BurstFireSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x076CAC0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool BurstFireSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, BurstFireSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x076CF10, this, SimTime, FrameTime, TransID, TransData);
	}

	void BurstFireSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, BurstFireSM*, EARS::StateMachineSys::StateMachine*>(0x073E4D0, this, &ChildMachine);
	}

	EARS::StateMachineSys::StateMachine* BurstFireSM::S_BurstFireSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new BurstFireSM(InID, InSMParams);
	}
}
