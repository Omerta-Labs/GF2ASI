#include "CEventHandler.h"

#include "Addons/Hook.h"

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
	MemUtils::CallClassMethod<void, RWS::CEventHandler*>(0x8517B230, this);
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
