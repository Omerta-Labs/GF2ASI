#include "base.h"

#include "Platform/MemUtils.h"

// SDK
#include "framework/core/simmanager/simmanager.h"

namespace EARS::Framework
{
	Base::Base(const RWS::CAttributePacket& InAttr)
		: RWS::CAttributeHandler()
		, RWS::CEventHandler()
		, SafeObj()
	{
		// TODO: Identify these flags
		m_FlagsAndID |= 0x28000000;
		m_EventHandlerFlags |= CEVENTHANDLER_FLAG_BASE;
		RegisterForDeleteNotification();

		SimManager::GetInstance()->AddStreamedEntityRecord(&InAttr, this);
	}

	Base::Base(const EARS::Common::guid128_t* InGuid, const uint32_t InStreamHandle)
		: RWS::CAttributeHandler()
		, RWS::CEventHandler()
		, SafeObj()
	{
		// TODO: Identify these flags
		m_FlagsAndID |= 0x28000000;
		m_EventHandlerFlags |= CEVENTHANDLER_FLAG_BASE;
		RegisterForDeleteNotification();

		// Select guid if incoming one is nullptr
		static EARS::Common::guid128_t PlaceholderGuid = EARS::Common::guid128_t(0xBEEF, 0xBEEF, 0xBEEF, 0xBEEF);
		const EARS::Common::guid128_t* EntityGuid = (InGuid != nullptr ? InGuid : &PlaceholderGuid);

		SimManager::GetInstance()->AddEntityRecord(m_InstanceId, nullptr, this, InStreamHandle);
	}

	Base::~Base()
	{
		// run through normal delete routines
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
		return InHandler.GetEventHandlerFlags() & CEVENTHANDLER_FLAG_BASE;
	}
}
