#include "City.h"

// SDK
#include "SDK/EARS_Framework/Core/SimManager/SimManager.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"

// Hook
#include "Addons/Hook.h"

bool EARS::Modules::City::IsKnownToPlayer() const
{
	return !m_Flags.Test((uint32_t)CityFlags::CITY_IS_HIDDEN_FROM_PLAYER);
}

void EARS::Modules::City::HideFromPlayer()
{
	m_Flags.Set((uint32_t)CityFlags::CITY_IS_HIDDEN_FROM_PLAYER);
}

void EARS::Modules::City::RevealToPlayer()
{
	if (m_Flags.Test((uint32_t)CityFlags::CITY_IS_HIDDEN_FROM_PLAYER))
	{
		m_Flags.Clear((uint32_t)CityFlags::CITY_IS_HIDDEN_FROM_PLAYER);
	}
}

void EARS::Modules::City::RequestTeleport() const
{
	RWS::LinkedEventHandlerIterator TestIt = RWS::LinkedEventHandlerIterator(m_TeleportMsg);
	while (!TestIt.IsFinished())
	{
		const RWS::CLinkedMsg* result = *TestIt;

		if (Base::IsEventHandlerBase(*result->m_EventHandler))
		{
			Base* AsBase = static_cast<Base*>(result->m_EventHandler);

			EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();

			auto guid = AsBase->InqInstanceID();
			RWS::CAttributePacket* pckt = SimMgr->GetAttributePacket(&guid, 0);

			int ClassID = pckt->GetIdOfClassToCreate();

			EARS::Framework::StreamManager* StreamMgr = EARS::Framework::StreamManager::GetInstance();
			const char* filename = StreamMgr->GetStreamFromHandle(pckt->GetStreamHandle())->GetFileName();

			int z = 0;
		}

		TestIt++;
	}

	// RWS::SendMsg
	MemUtils::CallCdeclMethod<void, RWS::CEventId*, bool>(0x0402050, &m_TeleportMsg, false);
}

String* EARS::Modules::City::GetDisplayName()
{
	return MemUtils::CallClassMethod<String*, City*>(0x0849530, this);
}
