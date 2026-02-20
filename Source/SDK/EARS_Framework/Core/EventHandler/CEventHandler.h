#pragma once

// EARS_Common
#include "SDK/EARS_Common/CommonTypes.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList2.h"

// CPP
#include <cstdint>

/**
 * NB: This is indeed from RenderWare's old Studio example project.
 * HOWEVER - This has been modified to meet the requirements of the GF2 engine.
 * So a simple copy and paste won't work here.
 */
namespace RWS
{
	class CEventHandler;

	template<typename TType>
	struct IntrusiveHashMixin
	{
	public:

		IntrusiveHashMixin()
			: m_HashNext(nullptr)
		{
			
		}

		TType* GetNext() const { return m_HashNext; }

		void SetNext(TType* InNext) { m_HashNext = InNext; }

	private:

		TType* m_HashNext = nullptr;
	};

	class CLinkedMsg 
		: public EARS::Common::DoubleLinkedListNodeMixin2<RWS::CLinkedMsg>
		, public RWS::IntrusiveHashMixin<RWS::CLinkedMsg>
	{
	public:

		RWS::CEventHandler* m_EventHandler = nullptr;	// TODO: Actually a union with next unlinked message
		uint32_t m_MsgId = 0;
		uint16_t m_Linked : 15;
		uint16_t m_bPendingUnlink : 1;
		uint16_t m_Priority = 0;

		struct MsgList 
			: public EARS::Common::DoubleLinkedListNodeMixin<RWS::CLinkedMsg::MsgList>
			, public EARS::Common::DoubleInternalLinkedList2<RWS::CLinkedMsg>
		{

		};
	};

	class CRegisteredMsgs : public RWS::IntrusiveHashMixin<CRegisteredMsgs>
	{
	public:

		uint32_t GetMsgID() const { return m_MsgID; }

		uint32_t GetKey() const { return GetMsgID(); }

		RWS::CLinkedMsg::MsgList* GetMsgListFront() const { return m_MsgList.GetFront(); }

	private:

		EARS::Common::DoubleInternalLinkedList<RWS::CLinkedMsg::MsgList> m_MsgList;
		char m_Padding[4];
		uint32_t m_MsgID;
	};

	class CEventId
	{
	public:

		uint32_t GetMsgId() const { return m_EventId; }

		RWS::CRegisteredMsgs* GetRegisteredInfo() const;

	private:

		uint32_t m_EventId = 0;
		uint16_t m_LinkedCount = 0;
		uint16_t m_RegisteredCount = 0;
	};

	class CMsg
	{
	public:

		// default constructor, considered to be invalid
		CMsg()
			: m_EventId(0)
			, m_EventData(nullptr)
			, m_bBroadcast(false)
		{

		}

		// message constructor with event ID but no data
		CMsg(const CEventId& InEventId)
			: m_EventId(InEventId.GetMsgId())
			, m_EventData(nullptr)
			, m_bBroadcast(false)

		{

		}

		// message constructor with event ID with data
		CMsg(const CEventId& InEventId, void* InData)
			: m_EventId(InEventId.GetMsgId())
			, m_EventData(InData)
			, m_bBroadcast(false)
		{

		}

		// Clear this message, does not destroy event data!
		void Clear();

		// Check whether this CMsg is of a specific type.
		bool IsEvent(const RWS::CEventId& Event) const;

		// NB: CONSIDER THIS UNSAFE ALWAYS! reinterpret_cast is extremely unsafe, with very little type safety.
		template<typename TDataType>
		const TDataType* GetDataAs() const
		{
			return reinterpret_cast<const TDataType*>(m_EventData);
		}

	private:

		uint32_t m_EventId = 0;
		void* m_EventData = nullptr;
		bool m_bBroadcast = false;
	};

	class CEventHandler
	{
	public:
		CEventHandler();
		virtual ~CEventHandler();

		virtual void HandleEvents(const RWS::CMsg& MsgEvent) {}
		virtual void DisableMessages();
		virtual void EnableMessages();


		void LinkMsg(CEventId* Msg, uint32_t Priority);
		void UnlinkMsg(CEventId* Msg);

		uint32_t GetEventHandlerFlags() const { return m_EventHandlerFlags; }

		bool IsActive() const;

		bool IsLightWeight() const;

		bool IsHeavyWeight() const;

		bool IsSuperHeavyWeight() const;

	private:

		enum class CEventHandlerFlags
		{
			CEVENTHANDLER_FLAG_ACTIVE = 1,
			CEVENTHANDLER_FLAG_IS_LIGHT_WEIGHT = 2,			// TODO: VALIDATE
			CEVENTHANDLER_FLAG_IS_HEAVY_WEIGHT = 4,
			CEVENTHANDLER_FLAG_IS_SUPER_HEAVY_WEIGHT = 8
		};

		uint32_t m_EventHandlerFlags = 0;
		void* m_SomeMsgUnion = nullptr; // $E7351B6BD7FB17CEB617F301D13C7028
	};

	struct LinkedEventHandlerIterator
	{
	public:

		LinkedEventHandlerIterator(const RWS::CEventId& InEventID);
		LinkedEventHandlerIterator(const RWS::CRegisteredMsgs& RegisteredMsgs);

		bool IsFinished();

		void WalkToNextEntry();

		void Reset();

		// operator overloads
		const RWS::CLinkedMsg* operator*();
		LinkedEventHandlerIterator& operator++(int a1);

	private:

		const RWS::CRegisteredMsgs* m_RegisteredMsgs = nullptr;
		RWS::CLinkedMsg::MsgList* m_CurrentList = nullptr;
		RWS::CLinkedMsg* m_Entry = nullptr;
	};
} // RWS

template<>
struct EARS::Common::HashNext<RWS::CRegisteredMsgs>
{
public:

	static RWS::CRegisteredMsgs* GetHashNext(const RWS::CRegisteredMsgs& Value)
	{
		return Value.GetNext();
	}

	static void SetHashNext(RWS::CRegisteredMsgs& Value, RWS::CRegisteredMsgs* Next)
	{
		Value.SetNext(Next);
	}
};

template<>
struct EARS::Common::GetKeyFunc<RWS::CRegisteredMsgs, uint32_t>
{
public:

	static uint32_t GetKey(const RWS::CRegisteredMsgs& Value)
	{
		return Value.GetKey();
	}
};
