#pragma once

// SDK (Common)
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/HashTable.h"
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Common/StaticArray.h"
#include "SDK/EARS_Common/String.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManagerTypes.h"

namespace EARS
{
	namespace Framework
	{
		// forward declarations
		class StreamTOC
		{
			uint32_t m_MagicNumber = 0;
			uint32_t m_Version = 0;
			uint16_t m_NumFiles = 0;
			uint16_t m_NumStreamRefs = 0;
			uint16_t m_NumSharedRefPtrs = 0;
			uint16_t m_RefCountAndFlags = 0;
		};

		/**
		 * Flags accepted by StreamManager::Load.
		 */
		enum StreamLoadFlags : uint32_t
		{
			STREAM_LOADFLAG_NONE = 0x0,

			/* Load "<DLPrefix><StreamName>.toc" and register it in the TOC array (marked as downloaded content). */
			STREAM_LOADFLAG_LOAD_TOC = 0x2,

			/* Passed by GFStreamAdditionHandler when loading DLC streams announced via .ads files. */
			STREAM_LOADFLAG_DOWNLOADED = 0x4,
		};

		struct StreamFileRef
		{
			EARS::Common::guid32_t m_Guid;
			const char* m_Filename = nullptr;
		};

		class StreamFile
		{
		public:

			const char* GetFileName() const { return m_StreamRef->m_Filename; }

			// TODO: Fix this crap
			EARS::Common::guid32_t GetGUID() const { return EARS::Common::guid32_t(m_StreamRef->m_Guid); }

		private:

			EARS::Framework::StreamFileRef* m_StreamRef = nullptr;
			uint32_t m_ContentSize = 0;
			uint8_t m_NumShared = 0;
			uint8_t m_NumForceLoad = 0;
			uint8_t m_Flags = 0;
			uint8_t m_pad = 0;
			uint32_t m_DLCIndex = 0;
			EARS::Framework::StreamFileRef** m_Relatives = nullptr;
		};

		/**
		 * Stores the state of a Stream File at runtime
		 */
		class Stream : public EARS::Common::DoubleLinkedListNodeMixin<Stream>
		{
		public:

			/* fetch the handle for this stream (active or inactive) */
			uint32_t GetStreamHandle() const { return m_StreamHandle; }

			/** get the current status of the stream */
			EARS::Framework::StreamStatus GetStatus() const { return m_Status; }

			/** return the Stream's filename */
			const char* GetFileName() const { return m_Filename.c_str(); }

		private:

			String m_Filename;
			const EARS::Framework::StreamFile* m_StreamNode = nullptr;
			uint32_t m_StreamHandle = 0;
			uint32_t m_LoadOrder = 0;
			EARS::Framework::StreamStatus m_Status = EARS::Framework::StreamStatus::STREAM_STATUS_INVALID;
			uint32_t m_UStreamHandle = 0;
			EARS::Common::guid32_t m_StreamGUID;


		};

		/**
		 * Handles the loading in and out of Stream Files at runtime
		 *
		 * Object is 0x160 bytes on PC. Known member offsets (from retail exe):
		 *   +0x44 Array<ReplacementRecord>    (items/size/capacity, capacity starts at 512)
		 *   +0xA4 StaticArray<StreamTOC*, 32> (count lives at +0x124)
		 *   +0x154 Array<String>              downloaded-content prefixes (registered via AddDevice)
		 */
		class StreamManager : public RWS::CEventHandler, public Singleton<EARS::Framework::StreamManager>
		{
		public:

			/**
			 * Registered by MarkStreamFileForReplacement; applied to every TOC as it loads
			 * (UpdateTOCDownloadFlags), so replacements survive later TOC loads.
			 */
			struct ReplacementRecord
			{
				EARS::Common::guid32_t m_StreamGUID;	// stream to replace; zero = match by name hash only
				uint32_t m_NameHash;					// SDBM(lowercase("<TOCRoot>\<file>.str")); 0 when matching by guid
				uint32_t m_PrefixIndex;					// index into the downloaded-content prefix array
				uint32_t m_TOCRootHash;					// SDBM of the TOC root name; 0 = any TOC
				uint32_t m_Version;						// when two records collide, highest version wins
			};

			/* request that a specific stream is loaded */
			uint32_t Load(uint32_t StreamGuid, float Priority, uint32_t LoadFlags);

			/**
			 * Request a stream (or TOC, see StreamLoadFlags) by name.
			 * Any file extension on StreamName is stripped and ".str" appended.
			 * When DLPrefix is provided the final path is "<DLPrefix><StreamName>.str",
			 * so the prefix must end with a path separator.
			 */
			uint32_t Load(const char* StreamName, float Priority, uint32_t LoadFlags, const EARS::Common::guid32_t* DLStreamGuid, const char* DLPrefix);

			/**
			 * Register a downloaded-content prefix (e.g. "mods\\mymod\\"). Idempotent.
			 * MUST be called before MarkStreamFileForReplacement with the same prefix:
			 * retail builds silently fall back to prefix index 0 for unknown prefixes.
			 */
			void AddDevice(const char* DevicePrefix);

			/**
			 * Redirect an existing stream to "<DLPrefix><original name>.str".
			 * Flags the StreamFile in every loaded TOC and stores a ReplacementRecord
			 * for TOCs loaded later. Returns true if an existing record was updated.
			 */
			bool MarkStreamFileForReplacement(EARS::Common::guid32_t StreamGuid, const String& DLPrefix, uint32_t NameHash, uint32_t TOCRootHash, uint32_t Version);

			/** Undo MarkStreamFileForReplacement (must pass the same arguments). */
			void ClearStreamFileForReplacement(EARS::Common::guid32_t StreamGuid, const String& DLPrefix, uint32_t NameHash, uint32_t TOCRootHash, uint32_t Version);

			/**
			 * Find a StreamFile across all registered TOCs, by guid or (optionally) name.
			 * Returns null until the TOC containing the stream has been loaded.
			 */
			const EARS::Framework::StreamFile* Lookup(EARS::Common::guid32_t StreamGuid, const char* OptionalName) const;

			/* fetch the stream handle from a Stream GUID (which can be found in StreamTOC) */
			uint32_t GetStreamHandle(uint32_t StreamGuid) const;

			/* fetch the stream handle for a stream filename (as produced by Load, e.g. "name.str") */
			uint32_t GetStreamHandle(const char* StreamFileName, bool bIsDownloaded) const;

			/** Resolve the Filename of the given Stream using its handle */
			const char* GetFilename(uint32_t InStreamHandle) const;

			/* Get the status of a stream, using their handle */
			EARS::Framework::StreamStatus GetStatus(const uint32_t StreamHandle) const;

			/* get the stream using their handle. */
			EARS::Framework::Stream* GetStreamFromHandle(const uint32_t InHandle) const;

			/** Get the stream which is currently being loaded (otherwise known as dispatched) */
			uint32_t GetDispatchStream() const;

			/** Incrememt dispatch lock counter for the current Dispatch in progress. */
			void AddDispatchLockComplete();

			/**
			 * Link the specific stream events to the handler.
			 * NB: This could be a static function call
			 */
			void LinkStreamMsg(const uint32_t StreamEventMask, RWS::CEventHandler& InHandler, const uint32_t Priority) const;

			/** Access the pending stream replacement records (see ReplacementRecord). */
			const Array<ReplacementRecord>& GetReplacementRecords() const;

			/** Number of registered TOCs (32 max). */
			uint32_t GetNumStreamTOCs() const;

			/** Fetch a registered TOC by index. Layout of StreamTOC is not reconstructed yet. */
			EARS::Framework::StreamTOC* GetStreamTOC(uint32_t Index) const;

			/** Access the downloaded-content prefixes registered via AddDevice. */
			const Array<String>& GetDownloadedContentPrefixes() const;

			/* get instance of stream manager */
			static StreamManager* GetInstance();

		private:

			char m_StreamManager_Padding0[0x34];
			Array<StreamManager::ReplacementRecord> m_ReplacementRecords;									// 0x44
			EARS::Common::HashTableByValue<uint32_t, EARS::Framework::Stream*, 64> m_HandleToStreamHash;	// 0x50
			EARS::Common::DoubleInternalLinkedList<EARS::Framework::Stream> m_DispatchedList;
			EARS::Common::DoubleInternalLinkedList<EARS::Framework::Stream> m_PendingList;
			EARS::Common::DoubleInternalLinkedList<EARS::Framework::Stream> m_UnloadList;
			EARS::Framework::Stream* m_ActiveStream = nullptr;
			EARS::Framework::Stream* m_DispatchStream = nullptr;
			char m_StreamManager_Padding2[0x14];
			uint32_t m_DispatchLockCount = 0;
			char m_StreamManager_Padding3[4];
			StaticArray<EARS::Framework::StreamTOC*, 32> m_TOCArray;
			StaticArray<uint32_t, 10> m_PendingAllDispatchedStreams;
			Array<String> m_DownloadPrefixes;
		};

		static_assert(sizeof(EARS::Framework::StreamManager) == 352);
	}
}
