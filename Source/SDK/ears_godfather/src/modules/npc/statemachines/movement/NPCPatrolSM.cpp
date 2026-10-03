#include "NPCPatrolSM.h"

namespace EARS::Modules
{
	NPCPatrolSM::NPCPatrolSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, NPCPatrolSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0761C30, this, TableID, SMParams);
	}

	NPCPatrolSM::~NPCPatrolSM()
	{
		MemUtils::CallClassMethod<void, NPCPatrolSM*>(0x0761B90, this);
	}

	bool NPCPatrolSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, NPCPatrolSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07627D0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool NPCPatrolSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, NPCPatrolSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0761D80, this, SimTime, FrameTime, TransID, TransData);
	}

	void NPCPatrolSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, NPCPatrolSM*, EARS::StateMachineSys::StateMachine*>(0x0761EA0, this, &ChildMachine);
	}

	int NPCPatrolSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, NPCPatrolSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* NPCPatrolSM::S_NPCPatrolSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new NPCPatrolSM(InID, InSMParams);
	}
}
