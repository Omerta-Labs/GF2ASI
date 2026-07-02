#include "Base.h"

#include "Addons/Hook.h"

namespace EARS::Framework
{
	Base::~Base()
	{
		// Default?
	}

	void Base::HandleEvents(const RWS::CMsg& MsgEvent)
	{
		// Forward to the engine's EARS::Framework::Base::HandleEvents
		MemUtils::CallClassMethod<void, const Base*, const RWS::CMsg&>(0x0046C6A0, this, MsgEvent);
	}

	void Base::DisableMessages()
	{
		RWS::CEventHandler::DisableMessages();

		if (HasComponents())
		{
			EnableMessagesToComponents();
		}
	}

	void Base::EnableMessages()
	{
		RWS::CEventHandler::EnableMessages();

		if (HasComponents())
		{
			DisableMessagesToComponents();
		}
	}

	bool Base::QueryInterface(const uint32_t ClassID, void** OutObjectPtr) const
	{
		*OutObjectPtr = nullptr;
		return false;
	}

	bool Base::IsEventHandlerBase(const RWS::CEventHandler& InHandler)
	{
		return InHandler.GetEventHandlerFlags() & (uint32_t)CEventHandlerFlags::CEVENTHANDLER_FLAG_BASE;
	}
}
