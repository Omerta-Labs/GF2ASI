#include "StreamManager.h"

// addons
#include "addons/Hook.h"

uint32_t EARS::Framework::StreamManager::Load(uint32_t StreamGuid, float Priority, uint32_t LoadFlags)
{
	return MemUtils::CallClassMethod<uint32_t, EARS::Framework::StreamManager*, uint32_t, float, uint32_t>(0x404280, this, StreamGuid, Priority, LoadFlags);
}

uint32_t EARS::Framework::StreamManager::GetStreamHandle(uint32_t StreamGuid) const
{
	return MemUtils::CallClassMethod<uint32_t, const EARS::Framework::StreamManager*, uint32_t>(0x04059B0, this, StreamGuid);
}

EARS::Framework::StreamStatus EARS::Framework::StreamManager::GetStatus(const uint32_t StreamHandle) const
{
	if (const EARS::Framework::Stream* const FoundStream = GetStreamFromHandle(StreamHandle))
	{
		return FoundStream->GetStatus();
	}

	return EARS::Framework::StreamStatus::STREAM_STATUS_INVALID;
}

EARS::Framework::Stream* EARS::Framework::StreamManager::GetStreamFromHandle(const uint32_t InHandle) const
{
	EARS::Framework::Stream* FoundStream = nullptr;
	if (m_HandleToStreamHash.Get(InHandle, &FoundStream))
	{
		return FoundStream;
	}

	return nullptr;
}

void EARS::Framework::StreamManager::LinkStreamMsg(const uint32_t StreamEventMask, RWS::CEventHandler& InHandler, const uint32_t Priority) const
{
	MemUtils::CallClassMethod<void, const EARS::Framework::StreamManager*, const uint32_t, RWS::CEventHandler&, const uint32_t>(0x0403620, this, StreamEventMask, InHandler, Priority);
}

EARS::Framework::StreamManager* EARS::Framework::StreamManager::GetInstance()
{
	return *(EARS::Framework::StreamManager**)0x122337C;
}
