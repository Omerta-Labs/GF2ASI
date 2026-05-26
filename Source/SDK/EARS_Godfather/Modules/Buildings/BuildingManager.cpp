#include "BuildingManager.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Buildings/Building.h"
#include "SDK/EARS_Godfather/Modules/Buildings/BuildingStore.h"

namespace EARS::Modules
{
	EARS::Modules::BuildingStore* BuildingManager::GetStoreByVenueID(uint32_t InVenueID) const
	{
		// first step is search loaded buildings; this will be quicker
		// instead of searching through full array
		if (const Building* AssociatedBuilding = GetBuildingByVenueID(InVenueID))
		{
			return AssociatedBuilding->GetStorageUnit();
		}
		
		// Otherwise the last resort is to iterate through registered store list
		if (BuildingStore** FoundStore = m_Stores.Search(InVenueID))
		{
			return *FoundStore;
		}

		return nullptr;
	}

	void BuildingManager::ForEachActiveBuilding(const TVisitActiveBuildingFunctor& InFunction)
	{
		for (const auto& RegData : m_ActiveBuildingList)
		{
			InFunction(*RegData.m_Data);
		}
	}

	void BuildingManager::ForEachBuildingStore(const TVisitBuildingStoreFunctor& InFunction)
	{
		for (const auto& RegData : m_Stores)
		{
			InFunction(*RegData.m_Data);
		}
	}

	Building* BuildingManager::GetBuildingByVenueID(const uint32_t InVenueID) const
	{
		if (Building** FoundBuilding = m_ActiveBuildingList.Search(InVenueID))
		{
			return *FoundBuilding;
		}

		return nullptr;
	}

	const char* BuildingManager::GetBuildingTypeInternalName(const BuildingTypeEnum InBuildingType) const
	{
		switch (InBuildingType)
		{
			case BuildingTypeEnum::BUSINESS: return "Business";
			case BuildingTypeEnum::RACKET: return "Racket";
			case BuildingTypeEnum::COMPOUND: return "Compound";
			case BuildingTypeEnum::DOCTOR: return "Doctor";
			case BuildingTypeEnum::BANK: return "Bank";
			case BuildingTypeEnum::SAFEHOUSE: return "Safehouse";
			case BuildingTypeEnum::FILLER: return "Filler";
			case BuildingTypeEnum::POLICE: return "Police";
			case BuildingTypeEnum::AIRPORT: return "Airport";
			case BuildingTypeEnum::HIT_HANGOUT: return "Hangout";
			default:
			{
				assert(false);
				return "Unknown";
			}
		}
	}

	/* static */
	BuildingManager* BuildingManager::GetInstance()
	{
		return *(BuildingManager**)0x112A558;
	}

}
