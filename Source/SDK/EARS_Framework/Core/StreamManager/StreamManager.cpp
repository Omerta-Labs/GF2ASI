#include "StreamManager.h"

// addons
#include <Addons/tConsole.h>
#include <Addons/Hook.h>

// C++
#include <assert.h>

uint32_t EARS::Framework::StreamManager::Load(uint32_t StreamGuid, float Priority, uint32_t LoadFlags)
{
	return MemUtils::CallClassMethod<uint32_t, EARS::Framework::StreamManager*, uint32_t, float, uint32_t>(0x404280, this, StreamGuid, Priority, LoadFlags);
}

uint32_t EARS::Framework::StreamManager::GetStreamHandle(uint32_t StreamGuid) const
{
	return MemUtils::CallClassMethod<uint32_t, const EARS::Framework::StreamManager*, uint32_t>(0x04059B0, this, StreamGuid);
}

const char* EARS::Framework::StreamManager::GetFilename(uint32_t InStreamHandle) const
{
	if (const EARS::Framework::Stream* Stream = GetStreamFromHandle(InStreamHandle))
	{
		return Stream->GetFileName();
	}

	return nullptr;
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

uint32_t EARS::Framework::StreamManager::GetDispatchStream() const
{
	if (m_DispatchStream)
	{
		return m_DispatchStream->GetStreamHandle();
	}

	return 0;
}

void EARS::Framework::StreamManager::AddDispatchLockComplete()
{
	m_DispatchLockCount++;

#if DEBUG
	const uint32_t StreamHandle = GetDispatchStream();
	assert(StreamHandle);

	const char* StreamFileName = GetFilename(StreamHandle);
	tConsole::fPrintf("StreamManager : AddLockDispatchComplete : %u : %s", m_DispatchLockCount, StreamFileName);
#endif // DEBUG
}

void EARS::Framework::StreamManager::LinkStreamMsg(const uint32_t StreamEventMask, RWS::CEventHandler& InHandler, const uint32_t Priority) const
{
	MemUtils::CallClassMethod<void, const EARS::Framework::StreamManager*, const uint32_t, RWS::CEventHandler&, const uint32_t>(0x0403620, this, StreamEventMask, InHandler, Priority);
}

EARS::Framework::StreamManager* EARS::Framework::StreamManager::GetInstance()
{
	return *(EARS::Framework::StreamManager**)0x122337C;
}
