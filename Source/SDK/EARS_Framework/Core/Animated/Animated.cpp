#include "Animated.h"

// SDK
#include "SDK/EARS_RT_CCT/include/ChrCntrl_AnimView.h"
#include "SDK/EARS_RT_CCT/include/ChrCntrl_Character.h"

// addons
#include "addons/Hook.h"

void EARS::Framework::Animated::SetSecondaryAnimSlots(uint32_t NumSlots)
{
	MemUtils::CallClassMethod<void, EARS::Framework::Animated*, uint32_t>(0x0460020, this, NumSlots);
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
