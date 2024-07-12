#pragma once

// CPP
#include <cstdint>

/**
 * NB: This is indeed from RenderWare's old Studio example project.
 * HOWEVER - This has been modified to meet the requirements of the GF2 engine.
 * So a simple copy and paste won't work here.
 */
namespace RWS
{
	class CLinkedMsg
	{

	};

	class CRegisteredMsgs
	{

	};

	class CEventId
	{
	public:

		uint32_t GetMsgId() const { return m_EventId; }

	private:

		uint32_t m_EventId = 0;
		uint16_t m_LinkedCount = 0;
		uint16_t m_RegisteredCount = 0;
	};

	class CMsg
	{
	public:

		bool IsEvent(const RWS::CEventId& Event) const;

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

	private:

		uint32_t m_EventHandlerFlags = 0;
		void* m_SomeMsgUnion = nullptr; // $E7351B6BD7FB17CEB617F301D13C7028
	};
} // RWS
