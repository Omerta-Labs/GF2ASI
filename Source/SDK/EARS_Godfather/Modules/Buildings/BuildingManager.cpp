#include "BuildingManager.h"

// Addons
#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Buildings/BuildingStore.h"

namespace EARS::Modules
{
	EARS::Modules::BuildingStore* BuildingManager::GetStoreByVenueID(uint32_t InVenueID) const
	{
		return MemUtils::CallClassMethod<BuildingStore*, const BuildingManager*, uint32_t>(0x06B0EE0, this, InVenueID);
	}

	/* static */
	BuildingManager* BuildingManager::GetInstance()
	{
		return *(BuildingManager**)0x112A558;
	}
}
