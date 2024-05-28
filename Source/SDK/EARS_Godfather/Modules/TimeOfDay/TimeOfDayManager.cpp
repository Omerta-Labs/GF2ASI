#include "TimeOfDayManager.h"

// CPP
#include <algorithm>

EARS::Modules::TimeOfDayManager::GameTime EARS::Modules::TimeOfDayManager::GetGameTime() const
{
	return m_BaseGameTime;
}

void EARS::Modules::TimeOfDayManager::SetGameTime(const GameTime& InGameTime)
{
	m_BaseGameTime.m_Year = std::clamp(InGameTime.m_Year, 0, 3000);
	m_BaseGameTime.m_Day = std::clamp(InGameTime.m_Day, 0, 364);
	m_BaseGameTime.m_Hour = std::clamp(InGameTime.m_Hour, 0, 23);
	m_BaseGameTime.m_Minute = std::clamp(InGameTime.m_Minute, 0, 59);
	m_BaseGameTime.m_Second = std::clamp(InGameTime.m_Second, 0, 59);

	m_ElapsedGameTicks = 0;
	m_ElapsedGameTicksCarriedOver = 0.0f;
}

EARS::Modules::TimeOfDayManager* EARS::Modules::TimeOfDayManager::GetInstance()
{
	return *(EARS::Modules::TimeOfDayManager**)0x112A53C;
}
