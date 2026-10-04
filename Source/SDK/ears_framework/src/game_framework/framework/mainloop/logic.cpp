#include "logic.h"

#include "Platform/MemUtils.h"

namespace RWS
{
	extern CEventId iMsgRunningTick = hook::Type<RWS::CEventId>(0x012069C4);
	extern CEventId iMsgPausedTick = hook::Type<RWS::CEventId>(0x12069B4);

	namespace MainLoop::Logic
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
	} //~ MainLoop::Logic
} //~ RWS
