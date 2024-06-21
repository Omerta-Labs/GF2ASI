#include "CityManager.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Turf/City.h"

int32_t EARS::Modules::CityManager::FindCityIndex(const uint32_t InCityID) const
{
	for (uint32_t i = 0; i < m_Cities.m_Size; i++)
	{
		EARS::Modules::City* CurrentCity = m_Cities[i];
		if (CurrentCity->GetCityID() == InCityID)
		{
			return i;
		}
	}
	
	return -1;
}

String* EARS::Modules::CityManager::GetDisplayName(const uint32_t CityID) const
{
	const int32_t CityIndex = FindCityIndex(CityID);
	if (CityIndex != -1)
	{
		return m_Cities[CityIndex]->GetDisplayName();
	}

	return nullptr;
}

void EARS::Modules::CityManager::ForEachCity(const TVisitCityFunctor& InFunction)
{
	for (uint32_t i = 0; i < m_Cities.Size(); i++)
	{
		EARS::Modules::City* CurrentCity = m_Cities[i];
		InFunction(*CurrentCity);
	}
}

EARS::Modules::CityManager* EARS::Modules::CityManager::GetInstance()
{
	return *(CityManager**)0x112A66C;
}


