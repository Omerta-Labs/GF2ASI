#include "NPCChaseSM.h"

namespace EARS::Modules
{
	NPCChaseSM::NPCChaseSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCChaseSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x075BA20, this, TableID, SMParams);
	}

	NPCChaseSM::~NPCChaseSM()
	{
		MemUtils::CallClassMethod<void, NPCChaseSM*>(0x046C640, this);
	}

	EARS::StateMachineSys::StateMachine* NPCChaseSM::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, NPCChaseSM*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x046C460, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void NPCChaseSM::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, NPCChaseSM*, uint32_t, float, uint32_t>(0x046C780, this, SimTime, FrameTime, TableID);
	}

	EARS::StateMachineSys::StateMachineSnapshot* NPCChaseSM::WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachineSnapshot*, NPCChaseSM*, EARS::StateMachineSys::StateMachineSnapshot*>(0x0461F30, this, pSnap);
	}

	int NPCChaseSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCChaseSM*, uint32_t, bool, bool, bool, float, bool>(0x046C740, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCChaseSM::S_NPCChaseSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCChaseSM(InID, InSMParams);
	}
}
