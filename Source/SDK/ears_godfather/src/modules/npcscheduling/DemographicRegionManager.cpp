#include "DemographicRegionManager.h"

void EARS::Modules::DemographicRegionManager::ForEachDemographicRegion(const TVisitDemographicRegionFunctor& InFunction) const
{
	for (const EARS::Modules::DemographicRegion* Region : m_RegisteredRegions)
	{
		InFunction(*Region);
	}
}

EARS::Modules::DemographicRegionManager* EARS::Modules::DemographicRegionManager::GetInstance()
{
	return *(EARS::Modules::DemographicRegionManager**)0x112A68C;
}
