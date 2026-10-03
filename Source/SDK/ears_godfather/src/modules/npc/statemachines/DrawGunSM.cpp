#include "DrawGunSM.h"

namespace EARS::Modules
{
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
}
