#include "PlayerF2FSM.h"

namespace EARS::Modules
{
	PlayerF2FSM::PlayerF2FSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerF2FSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07CE7C0, this, TableID, SMParams);
	}

	PlayerF2FSM::~PlayerF2FSM()
	{
		MemUtils::CallClassMethod<void, PlayerF2FSM*>(0x046C640, this);
	}

	EARS::StateMachineSys::StateMachine* PlayerF2FSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, PlayerF2FSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x046C460, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void PlayerF2FSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, PlayerF2FSM*, uint32_t, float, uint32_t>(0x046C780, this, SimTime, FrameTime, TableID);
	}

	EARS::StateMachineSys::StateMachineSnapshot* PlayerF2FSM::WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachineSnapshot*, PlayerF2FSM*, EARS::StateMachineSys::StateMachineSnapshot*>(0x0461F30, this, pSnap);
	}

	int PlayerF2FSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, PlayerF2FSM*, uint32_t, bool, bool, bool, float, bool>(0x046C740, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* PlayerF2FSM::S_PlayerF2FSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerF2FSM(InID, InSMParams);
	}
}
