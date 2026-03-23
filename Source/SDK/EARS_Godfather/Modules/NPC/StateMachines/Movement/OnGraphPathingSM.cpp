#include "OnGraphPathingSM.h"

namespace EARS::Modules
{
	OnGraphPathingSM::OnGraphPathingSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, OnGraphPathingSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x07631E0, this, TableID, SMParams);
	}

	OnGraphPathingSM::~OnGraphPathingSM()
	{
		MemUtils::CallClassMethod<void, OnGraphPathingSM*>(0x046C640, this);
	}

	EARS::StateMachineSys::StateMachine* OnGraphPathingSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, OnGraphPathingSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x046C460, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void OnGraphPathingSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, OnGraphPathingSM*, uint32_t, float, uint32_t>(0x046C780, this, SimTime, FrameTime, TableID);
	}

	EARS::StateMachineSys::StateMachineSnapshot* OnGraphPathingSM::WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachineSnapshot*, OnGraphPathingSM*, EARS::StateMachineSys::StateMachineSnapshot*>(0x0461F30, this, pSnap);
	}

	int OnGraphPathingSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, OnGraphPathingSM*, uint32_t, bool, bool, bool, float, bool>(0x046C740, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* OnGraphPathingSM::S_OnGraphPathingSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new OnGraphPathingSM(InID, InSMParams);
	}
}
