#include "CEventHandler.h"

#include "Addons/Hook.h"

#include "SDK/EARS_Common/HashTable.h"

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
	, m_SomeMsgUnion(nullptr)
{
	//m_SomeShit = MemUtils::CallCdeclMethod<void*>(0x409510);
	// TODO: This should match engine code
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

void RWS::CEventHandler::LinkMsg(CEventId* Msg, uint32_t Priority)
{
	MemUtils::CallCdeclMethod<void, RWS::CEventHandler*, CEventId*, uint32_t>(0x0408900, this, Msg, Priority);
}

void RWS::CEventHandler::UnlinkMsg(CEventId* Msg)
{
	MemUtils::CallClassMethod<void, RWS::CEventHandler*, CEventId*>(0x04086D0, this, Msg);
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
