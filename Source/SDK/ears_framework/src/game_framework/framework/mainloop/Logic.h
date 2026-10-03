#pragma once

// C++
#include <stdint.h>

namespace RWS::MainLoop::Logic
{
	extern void Running(uint32_t context);

	extern void Paused(uint32_t context);

	extern void PushPause(uint32_t context);

	extern void PopPause(uint32_t context);

	extern void Frozen();
}
