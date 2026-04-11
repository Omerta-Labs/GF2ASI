#pragma once

// C++
#include <stdint.h>

namespace EARS::Modules
{
	// forward declares
	class BuildingStore;

	class BuildingManager
	{
	public:

		/** Resolve a BuildingStore by its VenueID. */
		EARS::Modules::BuildingStore* GetStoreByVenueID(uint32_t InVenueID) const;

		/** get runtime instance of the building system */
		static BuildingManager* GetInstance();
	};
}