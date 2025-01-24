#pragma once

// SDK (Common)
#include "SDK/EARS_Common/DoubleInternalLinkedList.h"
#include "SDK/EARS_Common/HashTable.h"
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Common/String.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManagerTypes.h"

namespace EARS
{
	namespace Framework
	{


		class StreamFile
		{

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

		private:

			String m_Filename;
			const EARS::Framework::StreamFile* m_StreamNode = nullptr;
			uint32_t m_StreamHandle = 0;
			uint32_t m_LoadOrder = 0;
			EARS::Framework::StreamStatus m_Status = EARS::Framework::StreamStatus::STREAM_STATUS_INVALID;
			uint32_t m_UStreamHandle = 0;
			uint32_t m_StreamGUID = 0;


		};

		/**
		 * Handles the loading in and out of Stream Files at runtime
		 */
		class StreamManager : public RWS::CEventHandler, public Singleton<EARS::Framework::StreamManager>
		{
		public:

			/* request that a specific stream is loaded */
			uint32_t Load(uint32_t StreamGuid, float Priority, uint32_t LoadFlags);

			/* fetch the stream handle from a Stream GUID (which can be found in StreamTOC) */
			uint32_t GetStreamHandle(uint32_t StreamGuid) const;

			/* Get the status of a stream, using their handle */
			EARS::Framework::StreamStatus GetStatus(const uint32_t StreamHandle) const;

			/* get the stream using their handle. */
			EARS::Framework::Stream* GetStreamFromHandle(const uint32_t InHandle) const;

			/**
			 * Link the specific stream events to the handler.
			 * NB: This could be a static function call
			 */
			void LinkStreamMsg(const uint32_t StreamEventMask, RWS::CEventHandler& InHandler, const uint32_t Priority) const;

			/* get instance of stream manager */
			static StreamManager* GetInstance();

		private:

			char m_StreamManager_Padding0[0x40];
			EARS::Common::HashTableByValue<uint32_t, EARS::Framework::Stream*, 64> m_HandleToStreamHash;	// 0x50
			char m_StreamManager_Padding1[0x18];
			EARS::Framework::Stream* m_ActiveStream = nullptr;
		};
	}
}
