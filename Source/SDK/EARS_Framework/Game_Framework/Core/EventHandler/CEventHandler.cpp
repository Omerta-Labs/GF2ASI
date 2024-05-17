#include "CEventHandler.h"

#include "Addons/Hook.h"

bool RWS::CMsg::IsEvent(const RWS::CEventId& Event) const
{
	return m_EventId == Event.GetMsgId();
}

RWS::CEventHandler::CEventHandler()
	: m_EventHandlerFlags(1)
	, m_SomeShit(nullptr)
{
	//m_SomeShit = MemUtils::CallCdeclMethod<void*>(0x409510);
}

RWS::CEventHandler::~CEventHandler()
{

}

void RWS::CEventHandler::DisableMessages()
{
	m_EventHandlerFlags &= 0xFFFFFFFE;
}

void RWS::CEventHandler::EnableMessages()
{
	m_EventHandlerFlags |= 1;
}

void RWS::CEventHandler::LinkMsg(CEventId* Msg)
{
	MemUtils::CallClassMethod<void, RWS::CEventHandler*, CEventId*>(0x0408900, this, Msg);
}

void RWS::CEventHandler::UnlinkMsg(CEventId* Msg)
{
	MemUtils::CallClassMethod<void, RWS::CEventHandler*, CEventId*>(0x04086D0, this, Msg);
}
