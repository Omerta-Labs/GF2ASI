#include "GetToAndIntoVehicleSM.h"

namespace EARS::Modules
{
	GetToAndIntoVehicleSM::GetToAndIntoVehicleSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, GetToAndIntoVehicleSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0742AE0, this, TableID, SMParams);
	}

	GetToAndIntoVehicleSM::~GetToAndIntoVehicleSM()
	{
		MemUtils::CallClassMethod<void, GetToAndIntoVehicleSM*>(0x0743520, this);
	}

	bool GetToAndIntoVehicleSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, GetToAndIntoVehicleSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0744120, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool GetToAndIntoVehicleSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, GetToAndIntoVehicleSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0743A30, this, SimTime, FrameTime, TransID, TransData);
	}

	void GetToAndIntoVehicleSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, GetToAndIntoVehicleSM*, EARS::StateMachineSys::StateMachine*>(0x0743CB0, this, &ChildMachine);
	}

	int GetToAndIntoVehicleSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, GetToAndIntoVehicleSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* GetToAndIntoVehicleSM::S_GetToAndIntoVehicleSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new GetToAndIntoVehicleSM(InID, InSMParams);
	}
}
