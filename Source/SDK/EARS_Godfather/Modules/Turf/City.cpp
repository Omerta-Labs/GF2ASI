#include "City.h"

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
	// RWS::SendMsg
	MemUtils::CallCdeclMethod<void, RWS::CEventId*, bool>(0x0402050, &m_TeleportMsg, false);
}
