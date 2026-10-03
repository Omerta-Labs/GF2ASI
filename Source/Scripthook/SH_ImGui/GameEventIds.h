#pragma once

// RWS event ids the game registers at start-up, read from their fixed addresses.
//
// Each is `static`, so every translation unit gets its own hook::Type wrapper
// over the same address; the wrapper holds no state beyond that address.
//
// EARS_Framework's screenfx.cpp carries its own copy of RunningTickEvent,
// PausedTickEvent and DoRenderEvent. Both sets point at the same addresses, and
// the SDK is the right owner for them eventually.

#include "Platform/MemUtils.h"

#include "framework/core/eventhandler/ceventhandler.h"

namespace DefinedEvents
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	static hook::Type<RWS::CEventId> PausedTickEvent = hook::Type<RWS::CEventId>(0x12069B4);
	static hook::Type<RWS::CEventId> DoRenderEvent = hook::Type<RWS::CEventId>(0x01206970);
	static hook::Type<RWS::CEventId> PreRenderEvent = hook::Type<RWS::CEventId>(0x01206980);
	static hook::Type<RWS::CEventId> PlayerAsDriverEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E030);
	static hook::Type<RWS::CEventId> PlayerAsPassengerEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E11C);
	static hook::Type<RWS::CEventId> PlayerExitVehicleEvent = hook::Type<RWS::CEventId>(0x112E018);

	static hook::Type<RWS::CEventId> iMsgStreamLoadComplete = hook::Type<RWS::CEventId>(0x1206760);
	static hook::Type<RWS::CEventId> iMsgStreamUnloadComplete = hook::Type<RWS::CEventId>(0x1206768);
	static hook::Type<RWS::CEventId> iMsgStreamBeginUnload = hook::Type<RWS::CEventId>(0x1206778);
	static hook::Type<RWS::CEventId> iMsgStreamCancel = hook::Type<RWS::CEventId>(0x1206780);
	static hook::Type<RWS::CEventId> iMsgStreamAllDispatched = hook::Type<RWS::CEventId>(0x120678C);
	static hook::Type<RWS::CEventId> iMsgStreamBeginLoad = hook::Type<RWS::CEventId>(0x1206794);
	static hook::Type<RWS::CEventId> iMsgStreamIdle = hook::Type<RWS::CEventId>(0x12067A4);
	static hook::Type<RWS::CEventId> iMsgStreamUnloading = hook::Type<RWS::CEventId>(0x12067B4);

	static hook::Type<RWS::CEventId> iMsgPlayerTeleportDoneExceptFade = hook::Type<RWS::CEventId>(0x112B344);
}
