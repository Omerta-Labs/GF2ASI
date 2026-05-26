#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/RegArr.h"
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/ResourceManager/CResourceHandler.h"
#include "SDK/EARS_Framework/Core/Persistence/PersistenceRegistry.h"

// C++
#include <functional>
#include <stdint.h>

namespace EARS::Modules
{
	// forward declares
	class Building;
	class BuildingStore;

	enum class BuildingTypeEnum : int32_t
	{
		BuildingTypeEnum_REF = -1,
		BUSINESS = 0x0,
		RACKET = 0x1,
		COMPOUND = 0x2,
		DOCTOR = 0x3,
		BANK = 0x4,
		SAFEHOUSE = 0x5,
		FILLER = 0x6,
		POLICE = 0x7,
		AIRPORT = 0x8,
		HIT_HANGOUT = 0x9,
		BuildingTypeEnum_MAX_VALUE = 0xA,
	};

	class BuildingManager : 
		public Singleton<BuildingManager>, 
		public RWS::CEventHandler, 
		public RWS::CResourceHandler, 
		public EARS::Framework::IPersistable
	{
	public:

		/** Resolve a BuildingStore by its VenueID. */
		EARS::Modules::BuildingStore* GetStoreByVenueID(uint32_t InVenueID) const;

		const char* GetBuildingTypeInternalName(const BuildingTypeEnum InBuildingType) const;

		typedef std::function<void(Building&)> TVisitActiveBuildingFunctor;
		void ForEachActiveBuilding(const TVisitActiveBuildingFunctor& InFunction);

		typedef std::function<void(BuildingStore&)> TVisitBuildingStoreFunctor;
		void ForEachBuildingStore(const TVisitBuildingStoreFunctor& InFunction);

		/** get runtime instance of the building system */
		static BuildingManager* GetInstance();

	private:

		Building* GetBuildingByVenueID(const uint32_t InVenueID) const;

		Array<void*> m_BuildingResources; // EARS::Modules::BuildingResource
		RegArr<EARS::Modules::Building*, uint32_t> m_ActiveBuildingList;
		RegArr<EARS::Modules::BuildingStore*, uint32_t> m_Stores;
		uint32_t m_TypeCounts[10];
		String m_BuildingTypeNames[10];
	};
}
