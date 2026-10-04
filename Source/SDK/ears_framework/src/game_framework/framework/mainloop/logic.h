#pragma once

// SDK
#include "framework/core/eventhandler/ceventhandler.h"

// C++
#include <stdint.h>

namespace RWS
{
	RWS_DEFINE_EVENT(iMsgRunningTick, "RwUInt32", "Sent each logic loop while the game is unpaused,sends current frame number as parameter.");

	RWS_DEFINE_EVENT(iMsgPausedTick, "RwUInt32", "Sent each logic loop while the game is paused, sends current frame number as parameter.");

	namespace MainLoop::Logic
	{
		extern void Running(uint32_t context);

		extern void Paused(uint32_t context);

		extern void PushPause(uint32_t context);

		extern void PopPause(uint32_t context);

		extern void Frozen();
	}
}
