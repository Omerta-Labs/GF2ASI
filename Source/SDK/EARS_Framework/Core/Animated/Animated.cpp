#include "Animated.h"

// addons
#include "addons/Hook.h"

/* static */
EARS::Common::guid128_t EARS::Framework::Animated::FindModelStreamGuid(const RWS::CAttributePacket& AttributePacket)
{
	return MemUtils::CallCdeclMethod<EARS::Common::guid128_t, const RWS::CAttributePacket&>(0x045D9E0, AttributePacket);
}
