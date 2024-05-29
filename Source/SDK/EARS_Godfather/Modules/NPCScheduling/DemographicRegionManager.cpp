#include "DemographicRegionManager.h"

void EARS::Modules::DemographicRegionManager::ForEachDemographicRegion(const TVisitDemographicRegionFunctor& InFunction) const
{
	for (uint32_t i = 0; i < m_RegisteredRegions.Size(); i++)
	{
		const DemographicRegion* Region = m_RegisteredRegions[i];
		InFunction(*Region);
	}
}

EARS::Modules::DemographicRegionManager* EARS::Modules::DemographicRegionManager::GetInstance()
{
	return *(EARS::Modules::DemographicRegionManager**)0x112A68C;
}
