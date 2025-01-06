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
	class CAttributePacket : public EARS::Common::DoubleLinkedListNodeMixin2<CAttributePacket>
	{
	public:

		// Fetch the Stream Handle this AttributePacket was likely loaded by
		inline uint32_t GetStreamHandle() const { return m_hStream; }

		// Fetch the ID of the class to create.
		// This is stored within the data chunks.
		uint32_t GetIdOfClassToCreate() const;

		const EARS::Common::guid128_t& GetInstanceID() const;

		CAttributePacket* GetNext() const { return m_pHashNext; }
		void SetNext(CAttributePacket* InNext) { m_pHashNext = InNext; }

	private:

		uint32_t m_hStream = 0;
		CAttributePacket* m_PrevSibling = nullptr;
		CAttributePacket* m_NextSibling = nullptr;
		void* m_EntityList = nullptr;
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
