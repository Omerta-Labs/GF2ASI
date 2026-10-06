#pragma once

// SDK Common
#include "ears_common/BitArray.h"
#include "ears_common/commontypes.h"
#include "ears_common/doubleinternallinkedlist2.h"
#include "ears_common/guid.h"
#include "ears_common/rwtypes.h"

// SDK Framework
#include "framework/core/attributehandler/cclassfactory.h"
#include "framework/core/attributehandler/earsattributetypes.h"

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

	struct IDArray : BitArray<4096, uint32_t>
	{
		// nothing implemented
	};

	/**
	 * A list of entities associated one an other, using a linked list
	 * The list is stored within the packets, rather than a unique structure
	 * stored within the list. Use the iterator to search through the list.
	 */
	struct CAttributePacketEntityList
	{
	public:

		// Obtain the first handler associated with the packet
		CAttributeHandler* GetFront() const { return m_Head; }

		// Obtain the next handler in the sequence
		CAttributeHandler* GetNext() const;

		// check whether the list is empty
		bool IsEmpty() const { return m_Head == nullptr; }

		/**
		 * An iterator for simplify use
		 */
		struct Iterator
		{
		public:

			Iterator() = delete;
			Iterator(const RWS::CAttributePacketEntityList& InEntityList);

			// whether or not we've reached the end of the list
			bool IsFinished() const { return m_CurrentHandler == nullptr; }

			// fetch the current entity
			const RWS::CAttributeHandler* GetEntity() const { return m_CurrentHandler; }
			RWS::CAttributeHandler* GetEntity_Mutable() { return const_cast<RWS::CAttributeHandler*>(m_CurrentHandler); }

			// operator overloads
			const RWS::CAttributeHandler* operator*() { return GetEntity(); }
			Iterator& operator++(int a1);

		private:

			const RWS::CAttributeHandler* m_CurrentHandler = nullptr;
			const RWS::CAttributePacketEntityList* m_EntityList = nullptr;
		};

	private:

		// The start of the list
		RWS::CAttributeHandler* m_Head = nullptr;
	};

	namespace __Internal
	{
		struct CAttributeDataChunk
		{
			uint32_t m_Size = 0;
			uint32_t m_Type = 0;

			union
			{
				uint32_t m_RWS_DWORD;
				float m_Float;
				char char_;
				unsigned int uint32_t_;
				unsigned __int16 uint16_t_;
				unsigned __int8 uint8_t_;
				int int32_t_;
				__int16 int16_t_;
				char int8_t_;
				EARS::Common::guid128_t m_GUID;
				uint32_t RwRGBA_;						// RwRGBATag
				RwMatrixTag RwMatrixTagNoCtor_;			// RwMatrixTagNoCtor
				RwV3d RwV3d_;
			};
		};
	};

	struct CAttributeCommand
	{
	public:

		CAttributeCommand() { /* empty implementation */ }

		struct CompactData
		{
			const void* m_pData = nullptr;
			uint16_t m_CommandID = 0;
			uint16_t m_CompactTag = 0;
			uint32_t m_Data = 0;
		};

		uint32_t GetCommandId() const;

		bool IsCompact() const;

		const char* GetAs_char_ptr() const;

		EARS::Common::guid128_t* GetAs_RWS_GUID() const;

		float GetAs_float() const { return m_Chunk.m_Float; }

		uint32_t GetAs_uint32() const { return m_Chunk.uint32_t_; }

	private:

		union
		{
			CompactData m_CommandData;
			const RWS::__Internal::CAttributeDataChunk m_Chunk;
		};
	};

	static_assert(sizeof(CAttributeCommand) == 0x48);

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

	/**
	 * Iterate through the commands stored within the attribute packet.
	 */
	struct CAttributeCommandIterator
	{
	public:

		CAttributeCommandIterator() = delete;
		CAttributeCommandIterator(const CAttributePacket& InPacket, const uint32_t InTargetClassID);

		// Have we reached the end of the command buffer
		bool IsFinished() const;

		// Get the current ID of the command we're at
		uint32_t GetCommandID() const;

		// Query whether this command is actually used / set
		bool TestBit(uint32_t m_Idx) const;

		// Seek to a specific command within the buffer
		void SeekTo(const uint32_t NewIdx);

		const CAttributeDataChunk* GetDataChunk() const { return m_ChunkIterator.GetDataChunk(); }

		// operator overloads
		CAttributeDataChunkIterator& operator++(int a1);
		//const CAttributeDataChunk& operator*() const;
		const CAttributeCommand* operator->() const;
		//const CAttributeDataChunk* GetDataChunk(void) { return pCurrChunk_; }

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

		enum PacketFlags
		{
			PACKET_IS_COMPACT = 1,
			PACKET_COMPACT_IS_BASE = 2,
			PACKET_IS_DYNAMIC = 4,
			PACKET_HAS_COMPONENT_LIST = 8,
			PACKET_IS_OWNED_BY_SIMMANAGER = 16
		};

		// Fetch the Stream Handle this AttributePacket was likely loaded by
		inline uint32_t GetStreamHandle() const { return m_hStream; }

		// Fetch the ID of the class to create.
		// This is stored within the data chunks.
		uint32_t GetIdOfClassToCreate() const;

		// Check whether the Packet is compact
		bool IsCompact() const { return ((m_Flags & PacketFlags::PACKET_IS_COMPACT) == PacketFlags::PACKET_IS_COMPACT); }

		// Resolve EntityPacket only if PACKET_IS_COMPACT is true.
		const EARS::Framework::EntityPacket* EntPacket() const;

		// Get the instance of this packet
		const EARS::Common::guid128_t& GetInstanceId() const;

		// Check whether this Packet has any entities registered to them
		bool HasEntities() const { return (m_EntityList.IsEmpty() == false); }

		// Get the iterator for this packet
		CAttributePacketEntityList::Iterator GetEntityIterator() const;

		CAttributePacket* GetNext() const { return m_pHashNext; }
		void SetNext(CAttributePacket* InNext) { m_pHashNext = InNext; }

	private:

		uint32_t m_hStream = 0;
		CAttributePacket* m_PrevSibling = nullptr;
		CAttributePacket* m_NextSibling = nullptr;
		CAttributePacketEntityList m_EntityList;
		CAttributePacket* m_pHashNext = nullptr;

		// NB: This is actually EARS::Framework::EntityPacket!
		// Take this with an extreme pinch of salt!
		uint8_t m_Flags = 0;
		uint8_t m_pad[3];
		RWS::__Internal::CAttributeDataChunk m_FirstChunk;
	};

	class CAttributeHandler : public EARS::Common::DoubleLinkedListNodeMixin2<CAttributeHandler>
	{
	public:

		// NB: The release version does not have GetClassID nor does RWS_GetClassName!
		virtual ~CAttributeHandler();
		virtual void HandleAttributes(const RWS::CAttributePacket& InPacket);
		virtual void HandleAttributesFromProxy(const RWS::CAttributePacket& InPacket);
		virtual void DisableMessages() = 0;

		void EnableMessagesToComponents();
		void DisableMessagesToComponents();

		/** Fetch the Instance ID of this Attribute Handler. */
		const EARS::Common::guid128_t& InqInstanceID() const { return m_InstanceId; }

		/** Unpack the flags from the handler and return - useful to query specific flags */
		uint32_t GetAttributeHandlerFlags() const { return m_FlagsAndID & 0xFFFFF000; }

		bool HasAttributeHandlerFlag(const uint32_t InFlag) const;

		bool IsRuntimeIDValid() { return HasAttributeHandlerFlag(0x10000000); }
		bool IsManagedBySimManager() { return HasAttributeHandlerFlag(0x20000000); }

		uint32_t GetStream() const { return m_hStream; }

		bool HasComponents() const;
		EARS::Framework::Component* GetComponent(const uint32_t Index) const;

		// operator overloads
		void operator delete(void* Pointer, size_t Size);
		void* operator new(size_t Size, size_t AdditionalSize);	

		// construct-in-place: memory is already allocated + aligned, so just return it
		void* operator new(size_t, void* Ptr) noexcept { return Ptr; }
		void* operator new(size_t, std::align_val_t, void* Ptr) noexcept { return Ptr; }

		// matching placement deletes (only ever called if the constructor throws).
		// Nothing to free here — this operator new didn't allocate; the caller owns the memory.
		void  operator delete(void*, void*) noexcept {}
		void  operator delete(void*, std::align_val_t, void*) noexcept {}

	protected:	// following is available to derived types
		
		RWS::CAttributeHandler** m_PrevNextHandlerFromPacket = nullptr;
		RWS::CAttributeHandler* m_NextHandlerFromPacket = nullptr;
		uint32_t m_FlagsAndID = 0;
		uint32_t m_SubID = 0;
		uint32_t m_hStream = 0;
		EARS::Common::guid128_t m_InstanceId;

	private: // following is not

		void FreeRuntimeUID();

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
