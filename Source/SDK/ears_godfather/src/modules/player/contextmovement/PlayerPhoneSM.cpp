#include "PlayerPhoneSM.h"

namespace EARS::Modules
{
	PlayerPhoneSM::PlayerPhoneSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Modules::PlayerSM(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, PlayerPhoneSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07C2570, this, TableID, SMParams);
	}

	PlayerPhoneSM::~PlayerPhoneSM()
	{
		MemUtils::CallClassMethod<void, PlayerPhoneSM*>(0x046C640, this);
	}

	EARS::StateMachineSys::StateMachine* PlayerPhoneSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, PlayerPhoneSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x046C460, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void PlayerPhoneSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, PlayerPhoneSM*, uint32_t, float, uint32_t>(0x046C780, this, SimTime, FrameTime, TableID);
	}

	EARS::StateMachineSys::StateMachineSnapshot* PlayerPhoneSM::WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachineSnapshot*, PlayerPhoneSM*, EARS::StateMachineSys::StateMachineSnapshot*>(0x0461F30, this, pSnap);
	}

	int PlayerPhoneSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, PlayerPhoneSM*, uint32_t, bool, bool, bool, float, bool>(0x046C740, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* PlayerPhoneSM::S_PlayerPhoneSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new PlayerPhoneSM(InID, InSMParams);
	}
}
