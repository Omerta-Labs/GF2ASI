#include "CityManager.h"

void EARS::Modules::CityManager::ForEachCity(const TVisitCityFunctor& InFunction)
{
	for (uint32_t i = 0; i < m_Cities.Size(); i++)
	{
		City* City = m_Cities[i];
		InFunction(*City);
	}
}

EARS::Modules::CityManager* EARS::Modules::CityManager::GetInstance()
{
	return *(CityManager**)0x112A66C;
}
