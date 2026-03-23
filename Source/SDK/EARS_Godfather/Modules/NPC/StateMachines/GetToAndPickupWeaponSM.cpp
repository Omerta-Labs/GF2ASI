#include "GetToAndPickupWeaponSM.h"

namespace EARS::Modules
{
	GetToAndPickupWeaponSM::GetToAndPickupWeaponSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, GetToAndPickupWeaponSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0746490, this, TableID, SMParams);
	}

	GetToAndPickupWeaponSM::~GetToAndPickupWeaponSM()
	{
		MemUtils::CallClassMethod<void, GetToAndPickupWeaponSM*>(0x04DAF90, this);
	}

	bool GetToAndPickupWeaponSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, GetToAndPickupWeaponSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x0746780, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool GetToAndPickupWeaponSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, GetToAndPickupWeaponSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x07469C0, this, SimTime, FrameTime, TransID, TransData);
	}

	void GetToAndPickupWeaponSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, GetToAndPickupWeaponSM*, EARS::StateMachineSys::StateMachine*>(0x07468D0, this, &ChildMachine);
	}

	int GetToAndPickupWeaponSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, GetToAndPickupWeaponSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* GetToAndPickupWeaponSM::S_GetToAndPickupWeaponSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new GetToAndPickupWeaponSM(InID, InSMParams);
	}
}
