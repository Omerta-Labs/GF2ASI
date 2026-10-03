#include "CombatPathSM.h"

namespace EARS::Modules
{
	CombatPathSM::CombatPathSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: EARS::Framework::AnimateStateMachine(TableID, SMParams)
	{
		MemUtils::CallClassMethod<void, CombatPathSM*, uint32_t, EARS::StateMachineSys::StateMachineParams*>(0x0764900, this, TableID, SMParams);
	}

	CombatPathSM::~CombatPathSM()
	{
		MemUtils::CallClassMethod<void, CombatPathSM*>(0x04AC1E0, this);
	}

	bool CombatPathSM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, CombatPathSM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(0x07649B0, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	bool CombatPathSM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		return MemUtils::CallClassMethod<bool, CombatPathSM*, uint32_t, float, uint32_t, EARS::StateMachineSys::Transition::TransitionData*>(0x0764AF0, this, SimTime, FrameTime, TransID, TransData);
	}

	void CombatPathSM::InitialiseChild(EARS::StateMachineSys::StateMachine& ChildMachine)
	{
		MemUtils::CallClassMethod<void, CombatPathSM*, EARS::StateMachineSys::StateMachine*>(0x0764D80, this, &ChildMachine);
	}

	int CombatPathSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
	{
		return MemUtils::CallClassMethod<int, CombatPathSM*, uint32_t, bool, bool, bool, float, bool>(0x04AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
	}

	EARS::StateMachineSys::StateMachine* CombatPathSM::S_CombatPathSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new CombatPathSM(InID, InSMParams);
	}
}
