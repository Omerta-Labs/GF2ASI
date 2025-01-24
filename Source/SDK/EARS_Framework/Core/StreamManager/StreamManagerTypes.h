#pragma once

// SDK (Framework)
#include "SDK/EARS_Framework/Core/EventScheduler/IMsgData.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Framework
	{
		// forward declarations
		class Stream;

		enum StreamStatus : int32_t
		{
			STREAM_STATUS_INVALID = -1,
			STREAM_STATUS_QUEUED = 0x0,
			STREAM_STATUS_LOADING = 0x1,
			STREAM_STATUS_UNLOAD_QUEUE = 0x2,
			STREAM_STATUS_DISPATCHED = 0x3,
			STREAM_STATUS_UNLOADING = 0x4,
			STREAM_STATUS_FAILED = 0x5,
		};

		enum StreamEventID : uint32_t
		{
			STREAM_EV_INVALID = 0x0,
			STREAM_EV_BEGIN_LOAD = 0x1,
			STREAM_EV_LOAD_COMPLETE = 0x2,
			STREAM_EV_CANCEL = 0x4,
			STREAM_EV_EXPLICIT_DECREF = 0x8,
			STREAM_EV_BEGIN_UNLOAD = 0x10,
			STREAM_EV_UNLOADING = 0x20,
			STREAM_EV_UNLOAD_COMPLETE = 0x40,
			STREAM_EV_IDLE = 0x80,
			STREAM_EV_ALL_DISPATCHED = 0x100,
			STREAM_EV_ALL_MASK = 0x1FF,
		};

		struct StreamEvent : public EARS::Framework::IMsgData
		{
			enum class StreamEvStatus : uint32_t
			{
				STREAM_EV_STATUS_SUCCESS = 0x0,
				STREAM_EV_STATUS_INCOMPLETE = 0x1,
				STREAM_EV_STATUS_FAIL = 0x2,
				STREAM_EV_STATUS_BLOCKED = 0x3,
			};

			EARS::Framework::StreamEventID m_ID;
			EARS::Framework::StreamEvent::StreamEvStatus m_status;
			EARS::Framework::Stream* m_Stream = nullptr;
			uint32_t m_DispatchCount = 0;
		};
	}
}
