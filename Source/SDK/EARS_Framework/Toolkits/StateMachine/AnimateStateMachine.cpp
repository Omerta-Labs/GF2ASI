#include "AnimateStateMachine.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Framework/Core/Animated/Animated.h"
#include "SDK/EARS_RT_CCT/include/ChrCntrl_AnimView.h"
#include "SDK/EARS_StateMachine/StateMachineManager.h"

// ============================================================
// AnimateStateMachine
// ============================================================

EARS::Framework::AnimateStateMachine::AnimateStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
	: EARS::Framework::InterruptableStateMachine(TableID, SMParams)
{
	EARS::Framework::AnimateStateMachineParams* AnimateParams = static_cast<EARS::Framework::AnimateStateMachineParams*>(SMParams);
	m_AnimatedOwner = AnimateParams->GetOwner();
}

EARS::Framework::AnimateStateMachine::~AnimateStateMachine()
{
	// m_AnimatedOwner is cleared automatically via SafePtr destructor
}

// ------------------------------------------------------------
// StateMachine overrides
// ------------------------------------------------------------

bool EARS::Framework::AnimateStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
{
	// source: AnimateStateMachine::CheckTransition (PC ASM sub_4AC640) — clean switch reconstruction.
	switch (TransID)
	{
	case TRANSID_ANIMDONE:
	{
		return AnimDone();
	}
	case TRANSID_ANIMDONE_NEXTFRAME:
	{
		return IsLoopingAnim();
	}
	case TRANSID_SECONDARYANIMDONE:
	{
		// secondary anim slot index is stored at TransData+4
		const int Slot = *reinterpret_cast<const int*>(reinterpret_cast<const uint8_t*>(TransData) + 4);
		return SecondaryAnimDone(Slot);
	}
	default:
	{
		return EARS::Framework::InterruptableStateMachine::CheckTransition(SimTime, FrameTime, TransID, TransData);
	}
	}
}

bool EARS::Framework::AnimateStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
{
	// source: AnimateStateMachine::HandleStateMessage (PC ASM sub_4AC700).
	switch (MessageID)
	{
	case MESSAGE_PLAYANIM:
	{
		// MsgData+4 holds a pointer to an AnimMsg struct
		const AnimMsg* pAnim = static_cast<const AnimMsg*>(MsgData->GetPointerData());
		PlayAnim(pAnim->AnimID, pAnim->bBlend, pAnim->bForceAnim, pAnim->bIgnoreGameMovementBlend, pAnim->FrameRateScale, true);
		return true;
	}
	case MESSAGE_STOPANIM:
	{
		// Animated vtable slot 89 (PC offset 0x164) — 360 name TBD
		Animated* pAnimated = GetAnimated();
		uintptr_t* vftable = *reinterpret_cast<uintptr_t**>(pAnimated);
		reinterpret_cast<void(__thiscall*)(Animated*)>(vftable[89])(pAnimated);	// ApplyGameMovement()
		return true;
	}
	case MESSAGE_SETFRAMERATESCALE:
	{
		const float Speed = MsgData->GetFloatData();
		Animated* pAnimated = GetAnimated();
		pAnimated->SetAnimSpeed(Speed);
		return true;
	}
	default:
	{
		return EARS::Framework::InterruptableStateMachine::HandleStateMessage(SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}
	}
}

void EARS::Framework::AnimateStateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
{
	//// source: AnimateStateMachine::SpawnChild (PC ASM ?SpawnChild@AnimateStateMachine@Framework@EARS@@MAAXIMI@Z).
	//// Identical to base StateMachine::SpawnChild except it passes AnimateStateMachineParams
	//// with the resolved AnimatedOwner, so newly spawned child SMs inherit the Animated entity.
	using namespace EARS::StateMachineSys;

	AnimateStateMachineParams Params(GetAnimated());

	StateMachineManager* mgr = StateMachineManager::GetInstance();
	StateMachine* pChild = mgr->CreateStateMachineFromTableID(TableID, &Params);
	pChild->SetExternalTransitions(GetExternalTransitions());
	InitialiseChild(*pChild);
	AddChild(SimTime, FrameTime, pChild);
}

// ------------------------------------------------------------
// PlayAnim
// ------------------------------------------------------------

int EARS::Framework::AnimateStateMachine::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
{
	// source: AnimateStateMachine::PlayAnim (PC ASM sub_4AC300).
	return MemUtils::CallClassMethod<int, AnimateStateMachine*, uint32_t, bool, bool, bool, float, bool>(
		0x4AC300, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
}

// ------------------------------------------------------------
// Protected helpers
// ------------------------------------------------------------

bool EARS::Framework::AnimateStateMachine::AnimDone() const
{
	// source: PC ASM sub_4AC260
	Animated* pAnimated = GetAnimated();
	assert(pAnimated);

	return pAnimated->IsAnimDone();
}

bool EARS::Framework::AnimateStateMachine::IsLoopingAnim() const
{
	// source: PC ASM sub_4AC2A0 
	Animated* pAnimated = GetAnimated();
	assert(pAnimated);

	return pAnimated->IsPlayingLoopingAnim();
}

bool EARS::Framework::AnimateStateMachine::SecondaryAnimDone(int Slot) const
{
	// source: PC ASM sub_4AC2C0.
	const Animated* pAnimated = GetAnimated();
	assert(pAnimated);

	return pAnimated->IsSecondaryAnimDone(Slot);
}

int EARS::Framework::AnimateStateMachine::PlaySecondaryAnim(int Slot, uint32_t AnimID, bool bBlend, bool bForceAnim, bool bIgnoreGameMovementBlend, float FrameRateScale, bool bGameMovementTranslationScale, bool bUnk)
{
	// source: AnimateStateMachine::PlaySecondaryAnim (PC ASM sub_4AC480).
	return MemUtils::CallClassMethod<int, AnimateStateMachine*, int, uint32_t, bool, bool, bool, float, bool, bool>(
		0x4AC480, this, Slot, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale, bUnk);
}

int EARS::Framework::AnimateStateMachine::PlaySecondaryAnimWithoutTransition(int Slot, uint32_t AnimID, bool bBlend, bool bForceAnim, float FrameRateScale)
{
	// source: PC ASM sub_4AC530 — IDA: int __stdcall(int, int, char, char, float).
	// Mirrors PlaySecondaryAnim structure but with a reduced 5-arg signature on PC
	// (no bIgnoreGameMovementBlend, bGameMovementTranslationScale, bUnk).
	return MemUtils::CallClassMethod<int, AnimateStateMachine*, int, uint32_t, bool, bool, float>(
		0x4AC530, this, Slot, AnimID, bBlend, bForceAnim, FrameRateScale);
}

void EARS::Framework::AnimateStateMachine::ResetSecondaryAnimation(int Slot)
{
	// source: PC ASM sub_4AC600 — thin SafePtr wrapper; tail-calls sub_45F2A0
	// which traverses the secondary anim slot list and resets the entry at Slot.
	MemUtils::CallClassMethod<void, AnimateStateMachine*, int>(0x4AC600, this, Slot);
}

void EARS::Framework::AnimateStateMachine::ForceResetSecondaryAnimation(int Slot)
{
	// No distinct PC stub found adjacent to other AnimateStateMachine methods.
	// On PC, ForceReset likely maps to the same path as ResetSecondaryAnimation.
	ResetSecondaryAnimation(Slot);
}

bool EARS::Framework::AnimateStateMachine::IsPlayingSecondaryAnim(int Slot) const
{
	const Animated* pAnimated = GetAnimated();
	assert(pAnimated);

	return pAnimated->IsPlayingSecondaryAnim(Slot);
}

EARS::Framework::Animated* EARS::Framework::AnimateStateMachine::GetAnimated() const
{
	return m_AnimatedOwner.GetPtr();
}
