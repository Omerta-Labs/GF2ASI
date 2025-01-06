#include "CAttributeHandler.h"

// Hooks
#include <Addons/Hook.h>

uint32_t RWS::CAttributePacket::GetIdOfClassToCreate() const
{
	return MemUtils::CallClassMethod<uint32_t, const CAttributePacket*>(0x043AAF0, this);
}

const EARS::Common::guid128_t& RWS::CAttributePacket::GetInstanceID() const
{
	return MemUtils::CallClassMethod<const EARS::Common::guid128_t&, const RWS::CAttributePacket*>(0x043AB10, this);
}
