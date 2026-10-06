#include "ceventhandler.h"

#include "Platform/MemUtils.h"

#include "ears_common/hashtable.h"

#include <assert.h>

void RWS::CMsg::Clear()
{
	m_EventId = 0;
	m_EventData = nullptr;
	m_bBroadcast = false;
}

bool RWS::CMsg::IsEvent(const RWS::CEventId& Event) const
{
	return m_EventId == Event.GetMsgId();
}

RWS::CRegisteredMsgs* RWS::CMsg::GetRegisteredInfo() const
{
	// 01162380

	hook::Type<EARS::Common::IntrusiveHashTableFast<uint32_t, RWS::CRegisteredMsgs, 4096>*> EventTable = hook::Type<EARS::Common::IntrusiveHashTableFast<uint32_t, RWS::CRegisteredMsgs, 4096>*>(0x1162380);
	auto ptr = EventTable.get();

	return ptr->FindEntry(m_EventId);
}

RWS::CEventHandler::CEventHandler()
	: m_EventHandlerFlags(1)
	, m_LinkedMsgsLight(nullptr)
{

}

RWS::CEventHandler::~CEventHandler()
{
	// TODO: This should match engine code
	MemUtils::CallClassMethod<void, RWS::CEventHandler*>(0x04083D0, this);
}

void RWS::CEventHandler::DisableMessages()
{
	m_EventHandlerFlags &= 0xFFFFFFFE;
}

void RWS::CEventHandler::EnableMessages()
{
	m_EventHandlerFlags |= 1;
}

void RWS::CEventHandler::LinkMsg(RWS::CEventId& InId, const char* InFormatString, uint16_t InPriority)
{
	if (InId.IsValid())
	{
		LinkMsgToEventHandler(this, InId, InFormatString, InPriority);
	}
}

void RWS::CEventHandler::LinkMsg(RWS::CEventId& InId)
{
	if (InId.IsValid())
	{
		LinkMsgToEventHandler(this, InId, nullptr, MSG_PRIORITY_DEFAULT);
	}
}

void RWS::CEventHandler::LinkMsgToEventHandler(RWS::CEventHandler* InHandler, RWS::CEventId& InId, const char* InFormatString, uint16_t InPriority)
{
	// InFormatString only ever fed the debug build's event tooling; see RegisterMsg.
	(void)InFormatString;

	// The id's link count rises even when the handler was already linked, so it
	// counts links rather than linked handlers.
	InId.IncLinkedCount();

	if (RWS::CLinkedMsg* pLinkedMsg = InHandler->GetLinkedMsg(InId))
	{
		// Already linked: deepen the existing entry instead of adding a second one.
		// The original asserts here that InPriority matches the entry's priority -
		// a second link at a different priority silently keeps the first one's.
		pLinkedMsg->m_Linked = pLinkedMsg->m_Linked + 1;
	}
	else
	{
		InHandler->LinkMessageInternal(InId, InPriority);
	}
}

RWS::CLinkedMsg* RWS::CEventHandler::GetLinkedMsg(const RWS::CEventId& InId) const
{
	const uint32_t MsgId = InId.GetMsgId();

	// Only the light table is checked for null. A handler carrying either weight
	// flag has been through LinkMessageInternal, which never leaves the pointer
	// null once it has set them, and the engine relies on that here too.
	if (!IsHeavyWeight())
	{
		return (m_LinkedMsgsLight ? m_LinkedMsgsLight->Lookup(MsgId) : nullptr);
	}

	if (!IsSuperHeavyWeight())
	{
		return m_LinkedMsgsHeavy->Lookup(MsgId);
	}

	return m_LinkedMsgsSuperHeavy->Lookup(MsgId);
}

void RWS::CEventHandler::LinkMessageInternal(RWS::CEventId& InId, uint16_t InPriority)
{
	// TODO: still the engine's. The logic is recovered - allocate a CLinkedMsg
	// holding (this, InId.GetMsgId(), InPriority); create the 4-bin table if this
	// handler has none; Add to whichever table the weight flags select and, on
	// NEED_TO_GROW, allocate the next size up, assign the old table into it, free
	// the old one and set the next weight flag; then add the entry to
	// InId.GetRegisteredInfo(). What blocks writing it here is allocation: the
	// CLinkedMsg and all three table sizes come from EARS::Framework::FastPool
	// free lists, which this tree does not have yet, and the PC build inlined the
	// CLinkedMsg pool pop so there is no entry point to borrow for it either.
	//
	// `this` arrives in ESI rather than ECX, with the id and priority on the stack
	// and popped by the callee.
	MemUtils::CallEsiVoidMethod(0x04084D0, this, &InId, InPriority);
}

void RWS::CEventHandler::LinkMsgOnce(RWS::CEventId& InId)
{
	MemUtils::CallClassMethod<void, RWS::CEventHandler*, RWS::CEventId*>(0x0408680, this, &InId);
}

void RWS::CEventHandler::UnLinkMsg(RWS::CEventId& InId) const
{
	MemUtils::CallClassMethod<void, const RWS::CEventHandler*, RWS::CEventId*>(0x04086D0, this, &InId);
}

void RWS::CEventHandler::RegisterMsg(RWS::CEventId& InId, const char* InMsgName, const char* InFormatString)
{
	// Same story as LinkMsgToEventHandler: InFormatString survives in the original
	// signature but not in the PC image. 0x408240 hashes InMsgName and tail-calls
	// 0x408260, which reads only the id and the hash, so the optimiser dropped the
	// description and its callers push two arguments.
	(void)InFormatString;

	// Static, so a plain cdecl call with no this.
	MemUtils::CallCdeclMethod<void, RWS::CEventId*, const char*>(0x0408240, &InId, InMsgName);
}

void RWS::CEventHandler::UnRegisterMsg(RWS::CEventId& InId)
{
	MemUtils::CallCdeclMethod<void, RWS::CEventId*>(0x0408310, &InId);
}

void RWS::CEventHandler::ReplaceLinkedMsg(RWS::CEventId& InId, const char* InMsgName, const char* InFormatString)
{
	UnLinkMsg(InId);
	UnRegisterMsg(InId);

	if (InMsgName && *InMsgName)
	{
		RegisterMsg(InId, InMsgName, InFormatString);
		LinkMsg(InId, InFormatString, MSG_PRIORITY_DEFAULT);
	}
}

bool RWS::CEventHandler::IsActive() const
{
	return m_EventHandlerFlags & 1;
}

bool RWS::CEventHandler::IsLightWeight() const
{
	return m_EventHandlerFlags & 2;
}

bool RWS::CEventHandler::IsHeavyWeight() const
{
	return m_EventHandlerFlags & 4;
}

bool RWS::CEventHandler::IsSuperHeavyWeight() const
{
	return m_EventHandlerFlags & 8;
}

void RWS::CEventHandler::RegisterForDeleteNotification()
{
	m_EventHandlerFlags |= (uint32_t)CEventHandlerFlags::CEVENTHANDLER_FLAG_IS_LIGHT_WEIGHT;
}

RWS::CRegisteredMsgs* RWS::CEventId::GetRegisteredInfo() const
{
	// 01162380

	hook::Type<EARS::Common::IntrusiveHashTableFast<uint32_t, RWS::CRegisteredMsgs, 4096>*> EventTable = hook::Type<EARS::Common::IntrusiveHashTableFast<uint32_t, RWS::CRegisteredMsgs, 4096>*>(0x1162380);
	auto ptr = EventTable.get();

	return ptr->FindEntry(m_EventId);
}

RWS::LinkedEventHandlerIterator::LinkedEventHandlerIterator(const RWS::CEventId& InEventID)
	: LinkedEventHandlerIterator(*InEventID.GetRegisteredInfo())
{
}

RWS::LinkedEventHandlerIterator::LinkedEventHandlerIterator(const RWS::CRegisteredMsgs& RegisteredMsgs)
	: m_RegisteredMsgs(&RegisteredMsgs)
{
	Reset();
}

bool RWS::LinkedEventHandlerIterator::IsFinished()
{
	return (m_Entry == nullptr);
}

void RWS::LinkedEventHandlerIterator::WalkToNextEntry()
{
	if (m_Entry)
	{
		m_Entry = m_Entry->GetNextNode();
	}

	if (m_Entry == nullptr && m_CurrentList)
	{
		m_CurrentList = m_CurrentList->GetNext();
		if (m_CurrentList)
		{
			m_Entry = m_CurrentList->GetFront();
		}
	}
}

void RWS::LinkedEventHandlerIterator::Reset()
{
	m_Entry = nullptr;
	if (m_RegisteredMsgs)
	{
		m_CurrentList = m_RegisteredMsgs->GetMsgListFront();
		if (m_CurrentList)
		{
			m_Entry = m_CurrentList->GetFront();
		}
	}
	else
	{
		m_CurrentList = nullptr;
	}
}

const RWS::CLinkedMsg* RWS::LinkedEventHandlerIterator::operator*()
{
	return m_Entry;
}

RWS::LinkedEventHandlerIterator& RWS::LinkedEventHandlerIterator::operator++(int a1)
{
	WalkToNextEntry();
	return *this;
}

static bool _SendMsg(RWS::CMsg& InMsg, bool bSendToInactive)
{
	bool bResult = false;

	RWS::CRegisteredMsgs* RegisteredMsgs = InMsg.GetRegisteredInfo();
	if (RegisteredMsgs != nullptr && RegisteredMsgs->HasAnyListeners())
	{
		const bool bCurrentBroadcast = InMsg.IsBroadcasting();
		InMsg.SetBroadcasting(true);

		const bool bWasHandling = RegisteredMsgs->IsHandlingEvent();
		RegisteredMsgs->SetHandlingEvent(true);

		RWS::LinkedEventHandlerIterator HandlerIt = RWS::LinkedEventHandlerIterator(*RegisteredMsgs);
		while (!HandlerIt.IsFinished())
		{
			const RWS::CLinkedMsg* LinkedMsg = *HandlerIt;

			// Engine skips nodes queued for unlink (0x408A70: test word[+0x14], 0x8000).
			// m_EventHandler shares a union with m_NextPendingUnlink, so once a node is
			// pending unlink offset 0x0C is a link pointer, not a valid handler.
			if (LinkedMsg->HasValidEventHandler())
			{
				RWS::CEventHandler* Handler = LinkedMsg->m_EventHandler;
				if (Handler->IsActive() || bSendToInactive)
				{
					Handler->HandleEvents(InMsg);
				}
			}

			HandlerIt++;
		}

		RegisteredMsgs->SetHandlingEvent(bWasHandling);
		if (RegisteredMsgs->HasPendingUnlinks() && !bWasHandling)
		{
			// sub_408AF0 is __usercall: RegisteredMsgs is passed in EAX, not on the stack.
			MemUtils::CallEaxVoidMethod(0x408AF0, RegisteredMsgs);
		}

		InMsg.SetBroadcasting(bCurrentBroadcast);
		bResult = true;
	}

	return bResult;
}

bool RWS::SendMsg(const RWS::CEventId& InEventId, bool bSendToInactive)
{
	RWS::CMsg NewMsg = RWS::CMsg(InEventId);
	return _SendMsg(NewMsg, bSendToInactive);
	//return MemUtils::CallCdeclMethod<bool, const RWS::CMsg&, bool>(0x0408A00, NewMsg, bSendToInactive);
}

bool RWS::SendMsg(const RWS::CEventId& InEventId, void* InData, bool bSendToInactive)
{
	RWS::CMsg NewMsg = RWS::CMsg(InEventId, InData);
	return _SendMsg(NewMsg, bSendToInactive);
	//return MemUtils::CallCdeclMethod<bool, const RWS::CMsg&, bool>(0x0408A00, NewMsg, bSendToInactive);
}

bool RWS::SendMsg(RWS::CMsg& InMsg, bool bSendToInactive)
{
	return _SendMsg(InMsg, bSendToInactive);
	//return MemUtils::CallCdeclMethod<bool, RWS::CMsg&, bool>(0x0408A00, InMsg, bSendToInactive);
}

void RWS::CRegisteredMsgs::ProcessPendingUnlinks()
{
	// TODO: Implement
	assert(false);
	RWS::CLinkedMsg* FrontMsg = m_PendingUnlinks.GetFront();
	while (FrontMsg)
	{
		m_PendingUnlinks.Remove(FrontMsg);
	}
}
