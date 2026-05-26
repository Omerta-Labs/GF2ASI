#pragma once

#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Framework
#include "SDK/EARS_Common/BitFlags.h"
#include "SDK/EARS_Common/RwMaths.h"
#include "SDK/EARS_Common/String.h"

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class Building;
		class NPC;
		enum class BuildingTypeEnum;

		struct ISimNPCListener
		{
			//void(__cdecl* OnNPCSpawn)(EARS::Modules::ISimNPCListener* this, EARS::Modules::SimNpc*);
			//void(__cdecl* OnNPCPostSpawn)(EARS::Modules::ISimNPCListener* this, EARS::Modules::SimNpc*);
			//void(__cdecl* OnNPCDeath)(EARS::Modules::ISimNPCListener* this, EARS::Modules::SimNpc*);
			//void(__cdecl* OnNPCDelete)(EARS::Modules::ISimNPCListener* this, EARS::Modules::SimNpc*);
			//void(__cdecl* OnSimNPCDelete)(EARS::Modules::ISimNPCListener* this, EARS::Modules::SimNpc*);
			//void(__cdecl* GetDebugText)(EARS::Modules::ISimNPCListener* this, const EARS::Modules::SimNpc*, String*);

			virtual void OnNPCSpawn() = 0;
		};

		class BuildingStore : public ISimNPCListener, public RWS::CEventHandler
		{
		public:

			void ChangeOwnership(uint32_t NewFamilyID, const bool bShowFanfare, const void* pHeader, const bool bExtortion); /* pHeader -> EARS::Modules::BuildingPersistHeader */

			String* GetDisplayName();

			EARS::Modules::BuildingTypeEnum GetBuildingType() const { return m_BuildingType; }
			const RwV3d& GetCentrePos() const { return m_CentrePos; }
			uint32_t GetCityID() const { return m_CityID; }
			uint32_t GetDisplayNameID() const { return m_DisplayNameHashID; }
			const RwV3d& GetEntrancePos() const { return m_EntrancePos; }
			uint32_t GetFamilyID() const { return m_FamilyID; }
			const RwV3d& GetVehicleApproachPos() const { return m_VehicleApproachPos; }
			uint32_t GetVenueID() const { return m_VenueID; }

		private:

			RwV3d m_CentrePos;
			RwV3d m_EntrancePos;
			RwV3d m_VehicleApproachPos;
			Flags32 m_Flags;
			uint32_t m_VenueID = 0;
			uint32_t m_PreviousFamilyID = 0;
			uint32_t m_FamilyID = 0;
			uint32_t m_LastAttackerFamilyID = 0;
			uint32_t m_CityID = 0;
			char m_Padding_BuildingStore_0[116];
			void* m_Monopoly = nullptr;
			EARS::Modules::BuildingTypeEnum m_BuildingType;
			uint32_t m_BaseIncome = 0;
			uint32_t m_BonusIncome = 0;
			float m_RacketIncomeBonusMultiplier = 0.0f;
			EARS::Modules::Building* m_Building = nullptr;
			EARS::Modules::NPC* m_OwnerNPC = nullptr;
			uint32_t m_DisplayNameHashID = 0;
			String m_DisplayName;
			char m_Padding_BuildingStore_1[0x7C];
			uint32_t m_BuildingStateEnum = 0;
		};
	}
}