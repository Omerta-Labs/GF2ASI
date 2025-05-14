#include "Animated.h"

// addons
#include "addons/Hook.h"

void EARS::Framework::Animated::SetSecondaryAnimSlots(uint32_t NumSlots)
{
	MemUtils::CallClassMethod<void, EARS::Framework::Animated*, uint32_t>(0x0460020, this, NumSlots);
}

/* static */
EARS::Common::guid128_t EARS::Framework::Animated::FindModelStreamGuid(const RWS::CAttributePacket& AttributePacket)
{
	return MemUtils::CallCdeclMethod<EARS::Common::guid128_t, const RWS::CAttributePacket&>(0x045D9E0, AttributePacket);
}
