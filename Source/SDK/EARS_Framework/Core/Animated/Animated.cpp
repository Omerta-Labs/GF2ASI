#include "Animated.h"

// SDK
#include "SDK/EARS_RT_CCT/include/ChrCntrl_AnimView.h"
#include "SDK/EARS_RT_CCT/include/ChrCntrl_Character.h"
#include "SDK/EARS_RT_CCT/include/ChrCntrl_Secondary.h"

// addons
#include "addons/Hook.h"

void EARS::Framework::Animated::SetSecondaryAnimSlots(uint32_t NumSlots)
{
	MemUtils::CallClassMethod<void, EARS::Framework::Animated*, uint32_t>(0x0460020, this, NumSlots);
}

bool EARS::Framework::Animated::IsPlayingSecondaryAnim(int Slot) const
{
	return (m_AnimViewInfo && ChrCntl_Secondary_GetCurrStateId(m_AnimViewInfo, Slot) != -1);
}

bool EARS::Framework::Animated::IsPlayingAction(uint32_t ActionHash) const
{
	// source: sub_45EEC0 — PC address 0x45EEC0
	// Standard __thiscall: ecx=this (Animated*), stack arg=ActionHash
	//
	// PC ASM:
	//   mov  ecx, [ecx+218h]       ; m_AnimViewInfo
	//   cmp  [ecx+18h], esi        ; compare animView stateId with ActionHash
	//   mov  ecx, [ecx+70h]        ; secondary chain / action pointer
	//   cmp  [ecx+30h], 0          ; validate action state
	//   call sub_573440             ; action validation (non-standard eax calling convention)
	return MemUtils::CallClassMethod<bool, const EARS::Framework::Animated*, uint32_t>(
		0x45EEC0, this, ActionHash);
}

bool EARS::Framework::Animated::IsSecondaryAnimDone(int Slot) const
{
	assert(m_AnimViewInfo && HasSecondaryAnimChain(m_AnimViewInfo));

	if (m_AnimViewInfo && HasSecondaryAnimChain(m_AnimViewInfo))
	{
		return ChrCntl_Secondary_AnimEnd(m_AnimViewInfo, Slot);
	}

	return false;
}

bool EARS::Framework::Animated::IsPlayingLoopingAnim() const
{
	return (m_AnimViewInfo ? m_AnimViewInfo->IsLooping() : false);
}

bool EARS::Framework::Animated::IsAnimDone() const
{
	return (m_AnimViewInfo ? m_AnimViewInfo->IsAnimDone() : false);
}

void EARS::Framework::Animated::SetAnimSpeed(const float InSpeed)
{
	if (m_AnimViewInfo)
	{
		m_AnimViewInfo->SetAnimSpeed(InSpeed);
	}
}

const char* EARS::Framework::Animated::GetCharacterStateName() const
{
	const uint32_t CurrentStateId = m_AnimViewInfo->GetStateId();
	return EA::CCT::Character_GetStateName(*m_AnimViewInfo->GetCharacter(), CurrentStateId);
}

/* static */
EARS::Common::guid128_t EARS::Framework::Animated::FindModelStreamGuid(const RWS::CAttributePacket& AttributePacket)
{
	return MemUtils::CallCdeclMethod<EARS::Common::guid128_t, const RWS::CAttributePacket&>(0x045D9E0, AttributePacket);
}
