#include "NPCF2FStateMachine.h"

namespace EARS::Modules
{
	NPCF2FStateMachine::NPCF2FStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCF2FStateMachine*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0772B90, this, TableID, SMParams);
	}

	NPCF2FStateMachine::~NPCF2FStateMachine()
	{
		MemUtils::CallClassMethod<void, NPCF2FStateMachine*>(0x046C640, this);
	}

	EARS::StateMachineSys::StateMachine* NPCF2FStateMachine::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachine*, NPCF2FStateMachine*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x046C460, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	}

	void NPCF2FStateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
	{
		MemUtils::CallClassMethod<void, NPCF2FStateMachine*, uint32_t, float, uint32_t>(0x046C780, this, SimTime, FrameTime, TableID);
	}

	EARS::StateMachineSys::StateMachineSnapshot* NPCF2FStateMachine::WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap)
	{
		return MemUtils::CallClassMethod<EARS::StateMachineSys::StateMachineSnapshot*, NPCF2FStateMachine*, EARS::StateMachineSys::StateMachineSnapshot*>(0x0461F30, this, pSnap);
	}

	int NPCF2FStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCF2FStateMachine*, uint32_t, bool, bool, bool, float, bool>(0x046C740, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCF2FStateMachine::S_NPCF2FStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCF2FStateMachine(InID, InSMParams);
	}
}
