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

uint32_t EARS::Framework::StreamManager::Load(const char* StreamName, float Priority, uint32_t LoadFlags, const EARS::Common::guid32_t* DLStreamGuid, const char* DLPrefix)
{
	return MemUtils::CallClassMethod<uint32_t, EARS::Framework::StreamManager*, const char*, float, uint32_t, const EARS::Common::guid32_t*, const char*>(0x403A50, this, StreamName, Priority, LoadFlags, DLStreamGuid, DLPrefix);
}

void EARS::Framework::StreamManager::AddDevice(const char* DevicePrefix)
{
	MemUtils::CallClassMethod<void, EARS::Framework::StreamManager*, const char*>(0x4066A0, this, DevicePrefix);
}

bool EARS::Framework::StreamManager::MarkStreamFileForReplacement(EARS::Common::guid32_t StreamGuid, const String& DLPrefix, uint32_t NameHash, uint32_t TOCRootHash, uint32_t Version)
{
	return MemUtils::CallStdMethod<bool, EARS::Framework::StreamManager*, uint32_t, const String*, uint32_t, uint32_t, uint32_t>(0x4064D0, this, StreamGuid.ToUint32(), &DLPrefix, NameHash, TOCRootHash, Version);
}

void EARS::Framework::StreamManager::ClearStreamFileForReplacement(EARS::Common::guid32_t StreamGuid, const String& DLPrefix, uint32_t NameHash, uint32_t TOCRootHash, uint32_t Version)
{
	MemUtils::CallStdMethod<void, EARS::Framework::StreamManager*, uint32_t, const String*, uint32_t, uint32_t, uint32_t>(0x406380, this, StreamGuid.ToUint32(), &DLPrefix, NameHash, TOCRootHash, Version);
}

uint32_t EARS::Framework::StreamManager::GetStreamHandle(const char* StreamFileName, bool bIsDownloaded) const
{
	return MemUtils::CallClassMethod<uint32_t, const EARS::Framework::StreamManager*, const char*, bool>(0x405900, this, StreamFileName, bIsDownloaded);
}

const EARS::Framework::StreamFile* EARS::Framework::StreamManager::Lookup(EARS::Common::guid32_t StreamGuid, const char* OptionalName) const
{
	return MemUtils::CallClassMethod<const EARS::Framework::StreamFile*, const EARS::Framework::StreamManager*, uint32_t, const char*>(0x405720, this, StreamGuid.ToUint32(), OptionalName);
}

const Array<EARS::Framework::StreamManager::ReplacementRecord>& EARS::Framework::StreamManager::GetReplacementRecords() const
{
	return m_ReplacementRecords;
}

uint32_t EARS::Framework::StreamManager::GetNumStreamTOCs() const
{
	return m_TOCArray.GetSize();
}

EARS::Framework::StreamTOC* EARS::Framework::StreamManager::GetStreamTOC(uint32_t Index) const
{
	if (Index >= GetNumStreamTOCs())
	{
		return nullptr;
	}

	return m_TOCArray[Index];
}

const Array<String>& EARS::Framework::StreamManager::GetDownloadedContentPrefixes() const
{
	return m_DownloadPrefixes;
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
