#pragma once

// EARS_Common
#include "ears_common/commontypes.h"
#include "ears_common/hashtable.h"
#include "ears_common/doubleinternallinkedlist.h"
#include "ears_common/doubleinternallinkedlist2.h"
#include "ears_common/singleinternallinkedlist.h"

// CPP
#include <cstddef>
#include <cstdint>

// Example: RWS_DEFINE_EVENT(iMsgDoRender ,0,"Sent each frame to begin rendering.")
#define RWS_DEFINE_EVENT(name, type, desc) const char * const name##Str = #name; extern CEventId name

/**
 * NB: This is indeed from RenderWare's old Studio example project.
 * HOWEVER - This has been modified to meet the requirements of the GF2 engine.
 * So a simple copy and paste won't work here.
 */

namespace RWS
{
	class CEventHandler;

	// TODO: Find a home for this
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

		uint16_t GetPriority() const { return m_Priority; }

		// What IntrusiveHashTableFastCompact keys a handler's listener table on,
		// reached through EARS::Common::GetKeyFunc. CRegisteredMsgs carries the
		// same accessor for the same reason.
		uint32_t GetKey() const { return m_MsgId; }

		// NB: Just a guess
		bool HasValidEventHandler() const { return (m_bPendingUnlink == false); }

		union
		{
			RWS::CEventHandler* m_EventHandler;
			RWS::CLinkedMsg* m_NextPendingUnlink;
		};

		uint32_t m_MsgId = 0;

		// How many times this handler has linked the same message; unlinking drops
		// it by one and only the last unlink removes the entry.
		//
		// The declaration order matters and is not interchangeable with the reverse.
		// MSVC packs bitfields from the least significant bit on x86 and from the
		// most significant bit on PowerPC, so this one declaration puts m_Linked in
		// bits 0..14 on PC and in bits 1..15 on the 360. Reading the 360
		// disassembly alone suggests the fields are the other way round; they are
		// not, and swapping them to match it would corrupt the PC layout.
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

		// Raised on every link and lowered on every unlink, including repeat links of
		// the same id by one handler, so this counts links and not linked handlers.
		void IncLinkedCount() { m_LinkedCount++; }

		// An id is only usable once RegisterMsg has hashed a name into it. Both
		// LinkMsg overloads check this and do nothing when it is false, so linking
		// against an unregistered event is a silent no-op rather than an error.
		bool IsValid() const { return (m_EventId != 0); }

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


		// What the one-argument LinkMsg links at: the midpoint of the range
		// CLinkedMsg::m_Priority holds, so later links can sort either side of it.
		// Callers that care pass their own - the engine itself uses 0x7FFF and
		// 0x8001 in places. UNVERIFIED: the value is 0x8000 in both builds, but the
		// original's own name for the constant is not recovered.
		static constexpr uint16_t MSG_PRIORITY_DEFAULT = 0x8000;

		// Links this handler to InId so it receives that message. Does nothing when
		// InId is not yet registered. Linking the same id twice is allowed and
		// raises CEventId::m_LinkedCount rather than duplicating the entry.
		void LinkMsg(RWS::CEventId& InId, const char* InFormatString, uint16_t InPriority);

		// Links at MSG_PRIORITY_DEFAULT with no description string.
		void LinkMsg(RWS::CEventId& InId);

		void LinkMsgOnce(RWS::CEventId& InId);

		// Drops one link, so a handler linked twice stays linked once.
		void UnLinkMsg(RWS::CEventId& InId) const;

		// Gives InId its event id, derived from InMsgName by SDBM hash, and adds it
		// to the global registry. InFormatString is a description used only by the
		// debug build's event tooling.
		static void RegisterMsg(RWS::CEventId& InId, const char* InMsgName, const char* InFormatString);

		static void UnRegisterMsg(RWS::CEventId& InId);

		/**
		 * Rebinds this handler from whatever InId currently names to InMsgName: it
		 * unlinks, unregisters, then registers under the new name and links again at
		 * MSG_PRIORITY_DEFAULT. A null or empty InMsgName tears the binding down and
		 * stops there, which is how callers clear an event.
		 *
		 * This is what the attribute handlers call when a packet names a different
		 * event than the one already bound, hence the few hundred call sites in the
		 * PC image.
		 */
		void ReplaceLinkedMsg(RWS::CEventId& InId, const char* InMsgName, const char* InFormatString);

		uint32_t GetEventHandlerFlags() const { return m_EventHandlerFlags; }

		bool IsActive() const;

		bool IsLightWeight() const;

		bool IsHeavyWeight() const;

		bool IsSuperHeavyWeight() const;

		// Returns this handler's entry for InId, or nullptr when it is not linked.
		// Which of the three listener tables is consulted depends on the weight flags.
		RWS::CLinkedMsg* GetLinkedMsg(const RWS::CEventId& InId) const;

		void RegisterForDeleteNotification();

		// NB: In original game exe this implemented the logic.
		// But instead I have moved it into CRegisteredMsgs
		// The content of the function appears to be more suited abstracted in CRegisteredMsgs
		static void ProcessPendingUnlinks(RWS::CRegisteredMsgs& RegisteredMsg) { RegisteredMsg.ProcessPendingUnlinks(); }

	private:

		// Both LinkMsg overloads are a validity check and a call to this. It takes
		// the handler explicitly because it is static, which is also why the engine
		// reaches it directly from several hundred call sites.
		static void LinkMsgToEventHandler(RWS::CEventHandler* InHandler, RWS::CEventId& InId, const char* InFormatString, uint16_t InPriority);

		// Allocates a CLinkedMsg, files it in this handler's listener table - growing
		// that table a size if it reports NEED_TO_GROW - and adds it to the global
		// CRegisteredMsgs entry for the event. Only reached for a message this
		// handler is not already linked to.
		void LinkMessageInternal(RWS::CEventId& InId, uint16_t InPriority);

		// Promote the handler's listener table a size. Neither clears the lower flag:
		// IsHeavyWeight() stays true once super-heavy, and the checks are ordered to
		// rely on that.
		void SetHeavyWeight() { m_EventHandlerFlags |= (uint32_t)CEventHandlerFlags::CEVENTHANDLER_FLAG_IS_HEAVY_WEIGHT; }
		void SetSuperHeavyWeight() { m_EventHandlerFlags |= (uint32_t)CEventHandlerFlags::CEVENTHANDLER_FLAG_IS_SUPER_HEAVY_WEIGHT; }

	protected: // confirmed with DiaSymbolView

		enum class CEventHandlerFlags
		{
			CEVENTHANDLER_FLAG_ACTIVE = 1,
			CEVENTHANDLER_FLAG_IS_LIGHT_WEIGHT = 2,			// TODO: VALIDATE
			CEVENTHANDLER_FLAG_IS_HEAVY_WEIGHT = 4,
			CEVENTHANDLER_FLAG_IS_SUPER_HEAVY_WEIGHT = 8
		};

		uint32_t m_EventHandlerFlags = 0;
		
		union
		{
			RWS::IntrusiveHashTableFastCompact<unsigned int, RWS::CLinkedMsg, 4, 3, int, EARS::Common::CompareFunc<unsigned int>, EARS::Common::HashFunc<unsigned int>, EARS::Common::GetKeyFunc<RWS::CLinkedMsg, unsigned int>, EARS::Common::HashNext<RWS::CLinkedMsg> >* m_LinkedMsgsLight;
			RWS::IntrusiveHashTableFastCompact<unsigned int, RWS::CLinkedMsg, 32, 3, RWS::IntrusiveHashTableFastCompact<unsigned int, RWS::CLinkedMsg, 4, 3, int, EARS::Common::CompareFunc<unsigned int>, EARS::Common::HashFunc<unsigned int>, EARS::Common::GetKeyFunc<RWS::CLinkedMsg, unsigned int>, EARS::Common::HashNext<RWS::CLinkedMsg> >, EARS::Common::CompareFunc<unsigned int>, EARS::Common::HashFunc<unsigned int>, EARS::Common::GetKeyFunc<RWS::CLinkedMsg, unsigned int>, EARS::Common::HashNext<RWS::CLinkedMsg> >* m_LinkedMsgsHeavy;
			RWS::IntrusiveHashTableFastCompact<unsigned int, RWS::CLinkedMsg, 256, 5, RWS::IntrusiveHashTableFastCompact<unsigned int, RWS::CLinkedMsg, 32, 3, RWS::IntrusiveHashTableFastCompact<unsigned int, RWS::CLinkedMsg, 4, 3, int, EARS::Common::CompareFunc<unsigned int>, EARS::Common::HashFunc<unsigned int>, EARS::Common::GetKeyFunc<RWS::CLinkedMsg, unsigned int>, EARS::Common::HashNext<RWS::CLinkedMsg> >, EARS::Common::CompareFunc<unsigned int>, EARS::Common::HashFunc<unsigned int>, EARS::Common::GetKeyFunc<RWS::CLinkedMsg, unsigned int>, EARS::Common::HashNext<RWS::CLinkedMsg> >, EARS::Common::CompareFunc<unsigned int>, EARS::Common::HashFunc<unsigned int>, EARS::Common::GetKeyFunc<RWS::CLinkedMsg, unsigned int>, EARS::Common::HashNext<RWS::CLinkedMsg> >* m_LinkedMsgsSuperHeavy;
		};

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
