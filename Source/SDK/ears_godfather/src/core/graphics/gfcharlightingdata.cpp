#include "gfcharlightingdata.h"

namespace
{
	// EARS::Godfather::iMsgGFCharLightingDataUpdated
	constexpr uintptr_t ADDR_MSG_CHAR_LIGHTING_DATA_UPDATED = 0x11318CC;
}

const RWS::CEventId& EARS::Godfather::GFCharLightingData::GetDataUpdatedEvent()
{
	return *reinterpret_cast<const RWS::CEventId*>(ADDR_MSG_CHAR_LIGHTING_DATA_UPDATED);
}
