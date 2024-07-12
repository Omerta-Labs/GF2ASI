#pragma once

// SDK Common
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/String.h"

namespace EARS
{
	namespace Modules
	{
		class Sentient;

		enum class FamilyCategory : uint32_t
		{
			FamilyCategory_REF = 0xFFFFFFFF,
			FAMILY_CATEGORY_INNOCENT = 0x0,
			FAMILY_CATEGORY_WORKER = 0x1,
			FAMILY_CATEGORY_LAW = 0x2,
			FAMILY_CATEGORY_GANGSTER = 0x3,
			FamilyCategory_MAX_VALUE = 0x4,
		};

		/**
		 * An instance of a Family for Godfather II
		 */
		class Family
		{
		public:

			/**
			 * Add a Medic into this Family;
			 * Internally does some magic to ensure that killed members
			 * enter a 'revivable state', rather than instantly die
			 */
			void AddMedic(EARS::Modules::Sentient* NewMedic);

		private:

			char m_Padding_Family0[0x50]; // RWS::Base probably
			EARS::Modules::FamilyCategory m_Category = FamilyCategory::FamilyCategory_REF;
			uint32_t m_FamilyID = 0;
			uint32_t m_FamilyAllyID[3];
			uint32_t m_NumAllyFamilies = 0;
			int32_t m_WeaponMatchCount = 0;
			String m_RepChatterSuffixName;
			uint32_t m_SingularDisplayNameHashID = 0;
			uint32_t m_PluralDisplayNameHashID = 0;
			Flags32 m_Flags;
			uint32_t m_Group = 0;
			String m_InternalName;
			String m_SingularDisplayName;
			String m_PluralDisplayName;
			String m_VehicleTintName;
			EARS::Common::guid128_t m_FamilyVehicleGUID;
			EARS::Common::guid128_t m_FamilyChaseConfig;
			uint32_t m_NumMedics = 0;
			float m_PlayerGunDamageMultiplier = 0.0f;
			uint32_t m_FamilyMemberDefeatedScoreEvent = 0;
			uint32_t m_BuildingTintColor = 0; // RwRGBATag
			uint32_t m_SelectedBuildingTintColor = 0; // RwRGBATag
			float m_Balance = 0;
			float m_Income = 0;
			//Array<EARS::Modules::BuildingStore*> m_apOwnedRackets;
			//Array<EARS::Modules::BuildingStore*> m_apOwnedFronts;
			//unsigned int m_uNumPeakOwnedVenues;
			//Array<unsigned int> m_auCurrentMonopolyUpgrades;
			//Array<EARS::Modules::Family::Decision> m_aDecisions;
			//unsigned int m_uNumGuards;
			//unsigned int m_uNumGuardSlots;
			//float m_fGuardTransitTime;
			//float m_fMinTurnInterval;
			//float m_fMaxTurnInterval;
			//float m_fResponseDelay;
			//float m_fTimeSinceLastAction;
			//float m_fTimeSinceLastActedUpon;
			//bool m_bDesiresToRespond;
			//EARS::Modules::Family::Bonuses m_bonuses;
			//Array<EARS::Modules::Family::OmertaEntry> m_aOmertaTable;
			//float m_fMaxOmerta;
			//EARS::Modules::BuildingStore* m_pVenueConsidering;
			//EARS::Modules::Family* m_pFamilyConsidering;
			//Array<EARS::Modules::MadeMan*> m_apMadeMen;
			//float m_aRankPowerMappings[8];
			//const EARS::Modules::FamilyData* m_pData;
			//float m_fHospitalTime;
			//float m_fJailTime;
			//float m_fCooldownTime;
			//float m_fHangoutTime;
			//float m_fVenueBombingDelay;
			//RWS::CEventId m_performStingMsg;
			//RWS::CEventId m_jailbreakMsg;
			//RWS::CEventId m_quickRecoveryMsg;
			//RWS::CEventId m_unionHelpMsg;
			//RWS::CEventId m_lostAllVenuesMsg;
			//RWS::CEventId m_eliminateMsg;
			//RWS::CEventId m_gainedMonopolyMsg;
			//RWS::CEventId m_lostMonopolyMsg;
			//unsigned int m_uCompoundVenueID;
			//EARS::Modules::MoneyLedger* m_pMoneyLedger;

		};
	}
}
