#pragma once

// Convenience helpers for addressing RWS messages by name. These live outside the SDK
// on purpose: the SDK types stay engine-faithful, and the name<->hash coupling belongs
// to the mod layer (HashedNameRegistry).

// SDK
#include "SDK/ears_common/include/ears_common/commontypes.h"
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/ceventhandler.h"

// Scripthook
#include "Scripthook/SH_HashedNames/HashedNameRegistry.h"

namespace SH
{
	// Address a message by name: the name is hashed to an event id (and recorded in the
	// HashedNameRegistry), then dispatched exactly like RWS::SendMsg. Returns false if no
	// message with that id is registered in the engine event table.
	inline bool SendMsgByName(const char* Name, bool bSendToInactive = false)
	{
		const uint32_t Id = HashedNameRegistry::GetInstance().RegisterName(Name);
		const RWS::CEventId EventId(Id);
		return RWS::SendMsg(EventId, bSendToInactive);
	}

	inline bool SendMsgByName(const char* Name, void* InData, bool bSendToInactive = false)
	{
		const uint32_t Id = HashedNameRegistry::GetInstance().RegisterName(Name);
		const RWS::CEventId EventId(Id);
		return RWS::SendMsg(EventId, InData, bSendToInactive);
	}
}
