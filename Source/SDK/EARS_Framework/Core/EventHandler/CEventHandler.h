#pragma once

// EARS_Common
#include "SDK/EARS_Common/CommonTypes.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList2.h"
#include "SDK/EARS_Common/SingleInternalLinkedListLightweight.h"

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

		TType* GetHashNext() const { return m_HashNext; }

		void SetHashNext(TType* InNext) { m_HashNext = InNext; }

	private:

		TType* m_HashNext = nullptr;
	};

	class CLinkedMsg 
		: public EARS::Common::DoubleLinkedListNodeMixin2<RWS::CLinkedMsg>
		, public RWS::IntrusiveHashMixin<RWS::CLinkedMsg>
	{
	public:

		// To handle m_NextPendingUnlink linked list
		RWS::CLinkedMsg* GetNext() const { return m_NextPendingUnlink; }
		void SetNext(RWS::CLinkedMsg* InNext) { m_NextPendingUnlink = InNext; }

		// NB: Just a guess
		bool HasValidEventHandler() const { return (m_bPendingUnlink == false); }

		union
		{
			RWS::CEventHandler* m_EventHandler;
			RWS::CLinkedMsg* m_NextPendingUnlink;
		};

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

		void ProcessPendingUnlinks();

		uint32_t GetMsgID() const { return m_MsgID; }

		uint32_t GetKey() const { return GetMsgID(); }

		RWS::CLinkedMsg::MsgList* GetMsgListFront() const { return m_MsgList.GetFront(); }

		bool HasAnyListeners() const { return (m_MsgList.IsEmpty() == false); }

		bool IsHandlingEvent() const { return m_bHandlingEvent == 1; }
		void SetHandlingEvent(bool bValue) { m_bHandlingEvent = bValue; }

		bool HasPendingUnlinks() const { return (m_PendingUnlinks.IsEmpty() == false); }

	private:

		EARS::Common::DoubleInternalLinkedList<RWS::CLinkedMsg::MsgList> m_MsgList;
		EARS::Common::SingleInternalLinkedListLightweight<RWS::CLinkedMsg> m_PendingUnlinks;
		uint32_t m_MsgID = 0;
		uint16_t m_NumRegistered = 0;
		uint16_t m_bHandlingEvent : 1 = false;
		uint16_t m_NumLinked : 15 = 0;
	};

	static_assert(sizeof(CRegisteredMsgs) == 24);

	class CEventId
	{
	public:

		CEventId() = default;

		// Construct directly from an event id (i.e. an SDBM name hash). Lets callers
		// address a message by id without needing the engine's registered CEventId object.
		explicit CEventId(uint32_t InEventId)
			: m_EventId(InEventId)
		{
		}

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

		uint32_t GetEventID() const { return m_EventId; }

		bool IsBroadcasting() const { return m_bBroadcast; }
		void SetBroadcasting(bool bValue ) { m_bBroadcast = bValue; }

		RWS::CRegisteredMsgs* GetRegisteredInfo() const;

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

	extern bool SendMsg(const RWS::CEventId& InEventId, bool bSendToInactive);
	extern bool SendMsg(const RWS::CEventId& InEventId, void* InData, bool bSendToInactive);
	extern bool SendMsg(RWS::CMsg& InMsg, bool bSendToInactive);

	class CEventHandler
	{
	public:
		CEventHandler();
		virtual ~CEventHandler();

		virtual void HandleEvents(const RWS::CMsg& MsgEvent) { /* nothing by default */ }
		virtual void DisableMessages();
		virtual void EnableMessages();


		void LinkMsg(CEventId* Msg, uint32_t Priority);
		void UnlinkMsg(CEventId* Msg);

		uint32_t GetEventHandlerFlags() const { return m_EventHandlerFlags; }

		bool IsActive() const;

		bool IsLightWeight() const;

		bool IsHeavyWeight() const;

		bool IsSuperHeavyWeight() const;

		// NB: In original game exe this implemented the logic.
		// But instead I have moved it into CRegisteredMsgs
		// The content of the function appears to be more suited abstracted in CRegisteredMsgs
		static void ProcessPendingUnlinks(RWS::CRegisteredMsgs& RegisteredMsg) { RegisteredMsg.ProcessPendingUnlinks(); }

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
		return Value.GetHashNext();
	}

	static void SetHashNext(RWS::CRegisteredMsgs& Value, RWS::CRegisteredMsgs* Next)
	{
		Value.SetHashNext(Next);
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
