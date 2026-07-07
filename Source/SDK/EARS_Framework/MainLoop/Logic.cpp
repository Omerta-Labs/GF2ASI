#include "Logic.h"

#include "addons/hook.h"

namespace RWS::MainLoop::Logic
{
	void Running(uint32_t context)
	{
		MemUtils::CallCdeclMethod<void>(0x40EA60, context);
	}

	void Paused(uint32_t context)
	{
		MemUtils::CallCdeclMethod<void>(0x40EAA0, context);
	}

	void PushPause(uint32_t context)
	{
		MemUtils::CallCdeclMethod<void>(0x40EAC0, context);
	}

	void PopPause(uint32_t context)
	{
		MemUtils::CallCdeclMethod<void>(0x40EAF0, context);
	}

	void Frozen()
	{
		MemUtils::CallCdeclMethod<void>(0x40EB90);
	}
}
