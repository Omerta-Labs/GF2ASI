#include "CivilianVehicleStateMachine.h"

namespace EARS::Modules
{
	CivilianVehicleStateMachine::CivilianVehicleStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, CivilianVehicleStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0873570, this, TableID, SMParams);
	}

	CivilianVehicleStateMachine::~CivilianVehicleStateMachine()
	{
		MemUtils::CallClassMethod<void, CivilianVehicleStateMachine*>(0x04DAF90, this);
	}

	bool CivilianVehicleStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CivilianVehicleStateMachine*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x08736B0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CivilianVehicleStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CivilianVehicleStateMachine*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0873500, this, SimTime, FrameTime, TransID, TransData);
	}

	void CivilianVehicleStateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, CivilianVehicleStateMachine*, uint32_t, float, uint32_t>(0x086B0E0, this, SimTime, FrameTime, TableID);
	}

	int CivilianVehicleStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, CivilianVehicleStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* CivilianVehicleStateMachine::S_CivilianVehicleStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CivilianVehicleStateMachine(InID, InSMParams);
	}
}
