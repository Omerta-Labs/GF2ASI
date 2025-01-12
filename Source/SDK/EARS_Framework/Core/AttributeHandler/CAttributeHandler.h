#pragma once

// SDK Common
#include "SDK/EARS_Common/CommonTypes.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList2.h"
#include "SDK/EARS_Common/Guid.h"

// Forward declare
namespace EARS
{
	namespace Framework
	{
		class Component;
		class ComponentListRecord;
	}
}

namespace RWS
{
	class CAttributePacket;
	class CAttributeHandler;

	struct CAttributePacketEntityList
	{
	public:

		// Obtain the first handler associated with the packet
		CAttributeHandler* GetFront() const { return m_Head; }

		// Obtain the next handler in the sequence
		CAttributeHandler* GetNext() const;

		// check whether the list is empty
		bool IsEmpty() const { return m_Head == nullptr; }

		struct Iterator
		{
		public:

			Iterator() = delete;
			Iterator(const RWS::CAttributePacketEntityList& InEntityList);

			bool IsFinished() const { return m_CurrentHandler; }

			const RWS::CAttributeHandler* GetObject() const { return m_CurrentHandler; }

			// operator overloads
			const RWS::CAttributeHandler* operator*() { return GetObject(); }

			Iterator& operator++(int a1);

		private:

			const RWS::CAttributeHandler* m_CurrentHandler = nullptr;
			const RWS::CAttributePacketEntityList* m_EntityList = nullptr;
		};

	private:

		CAttributeHandler* m_Head = nullptr;
	};

	struct CAttributeCommand
	{
	public:

		const char* GetAs_char_ptr() const;

	private:

		// TODO: This is a union, but im too lazy to implement union right now
		char m_Padding[0x48];
	};

	struct CAttributeDataChunk
	{
	public:

	private:

		uint32_t m_Size = 0;
		uint32_t m_Type = 0;
		char m_Padding[0x40];
	};

	struct CAttributeDataChunkIterator
	{
	public:

		const CAttributeDataChunk* GetDataChunk() const { return m_DataChunk; }

	private:

		const CAttributeDataChunk* m_DataChunk = nullptr;
	};

	struct CAttributeCommandIterator
	{
	public:

		bool IsFinished() const;

		uint32_t GetCommandID() const { return m_CurIdx; }

		bool TestBit(uint32_t m_Idx) const;

		const CAttributeDataChunk* GetDataChunk() const { return m_ChunkIterator.GetDataChunk(); }

		CAttributeDataChunkIterator& operator++(int a1);

		//const CAttributeDataChunk& operator*() const;

		const CAttributeCommand* operator->() const;

		//const CAttributeDataChunk* GetDataChunk(void) { return pCurrChunk_; }

		static CAttributeCommandIterator MakeIterator(const RWS::CAttributePacket& InPacket, uint32_t InTargetClassID);

	private:

		bool m_bIsCompact = false;
		const uint8_t* m_ZeroValueBitVec = nullptr;
		const uint32_t* m_AttrDataCursor = nullptr;
		int32_t m_CurIdx = -1;
		int32_t m_NumAttrs = -1;
		RWS::CAttributeCommand m_CompactCommand;
		RWS::CAttributeDataChunkIterator m_ChunkIterator;
		uint32_t m_TargetClassID = 0;

	};

	class CAttributePacket : public EARS::Common::DoubleLinkedListNodeMixin2<CAttributePacket>
	{
	public:

		// Fetch the Stream Handle this AttributePacket was likely loaded by
		inline uint32_t GetStreamHandle() const { return m_hStream; }

		// Fetch the ID of the class to create.
		// This is stored within the data chunks.
		uint32_t GetIdOfClassToCreate() const;

		const EARS::Common::guid128_t& GetInstanceID() const;

		// Check whether this Packet has any entities registered to them
		bool HasEntities() const { return (m_EntityList.IsEmpty() == false); }

		CAttributePacketEntityList::Iterator GetEntityIterator() const;

		CAttributePacket* GetNext() const { return m_pHashNext; }
		void SetNext(CAttributePacket* InNext) { m_pHashNext = InNext; }

	private:

		uint32_t m_hStream = 0;
		CAttributePacket* m_PrevSibling = nullptr;
		CAttributePacket* m_NextSibling = nullptr;
		CAttributePacketEntityList m_EntityList;
		CAttributePacket* m_pHashNext = nullptr;
		uint8_t m_Flags = 0;
		uint8_t m_pad[3];
		//RWS::__Internal::CAttributeDataChunk firstChunk_;
	};

	class CAttributeHandler : public EARS::Common::DoubleLinkedListNodeMixin2<CAttributeHandler>
	{
	public:

		virtual ~CAttributeHandler() = 0;
		virtual void HandleAttributes() = 0;
		virtual void HandleAttributesFromProxy() = 0;
		virtual void DisableMessages() = 0;

		/**
		 * Fetch the Instance ID of this Attribute Handler.
		 */
		EARS::Common::guid128_t InqInstanceID() const { return m_InstanceId; }

	private:

		RWS::CAttributeHandler** m_PrevNextHandlerFromPacket = nullptr;
		RWS::CAttributeHandler* m_NextHandlerFromPacket = nullptr;
		uint32_t m_FlagsAndID = 0;
		uint32_t m_SubID = 0;
		uint32_t m_hStream = 0;
		EARS::Common::guid128_t m_InstanceId;

		// TODO: Figure out whether or not this is correct
		char m_AttributeHandler_Padding[0x4];
		EARS::Framework::ComponentListRecord* m_ComponentList = nullptr;
		EARS::Framework::Component** m_Components = nullptr;

		friend CAttributePacketEntityList;
		friend CAttributePacketEntityList::Iterator;
	};
} // EARS

template<>
struct EARS::Common::HashNext<RWS::CAttributePacket>
{
public:

	static RWS::CAttributePacket* GetHashNext(const RWS::CAttributePacket& Value)
	{
		return Value.GetNext();
	}

	static void SetHashNext(RWS::CAttributePacket& Value, RWS::CAttributePacket* Next)
	{
		Value.SetNext(Next);
	}
};
