#pragma once

// SDK Framework
#include "SDK/EARS_Framework/Core/Base/Base.h"

// SDK Common
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/String.h"

// SDK Godfather
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/Scoring/MoneyLedger.h"

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
		class Family : public EARS::Framework::Base
		{
		public:

			/**
			 * Add a Medic into this Family;
			 * Internally does some magic to ensure that killed members
			 * enter a 'revivable state', rather than instantly die
			 */
			void AddMedic(EARS::Modules::Sentient* NewMedic);

			/**
			 * Adjust the balance of a specific LedgerType in the Money Ledger, 
			 * only works if the Family has a concept of Money and balance
			 * (not all of them do)
			 * @param Modifier - The amount to adjust the Ledger value
			 * @param LedgerType - The associated type of Ledger to adjust
			 * @return float - Balance added together?
			 */
			float ModifyBalance(const float Modifier, const LedgerItemType LedgerType);

			// getters
			inline uint32_t GetFamilyID() const { return m_FamilyID; }

		private:

			enum Action : int32_t
			{
				ACTION_INVALID = 0x0,
				ACTION_VENUE_ATTACK = 0x1,
				ACTION_VENUE_BOMB = 0x2,
				ACTION_RESPONDER_MOVE = 0x4,
				ACTIONS_OFFENSIVE = 0x3,
			};

			struct Decision
			{
				float m_Weight = 0.0f;
				EARS::Modules::Family::Action m_Action = Action::ACTION_INVALID;
				void* m_pVenueOfInterest; // EARS::Modules::BuildingStore
				bool m_IsPlayerInferred = false;
				Array<EARS::Modules::SimNPC*> m_RespondersToUse;
			};

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
			//float m_Balance = 0;
			//float m_Income = 0;
			//Array<void*> m_OwnedRackets; // EARS::Modules::BuildingStore
			//Array<void*> m_OwnedFronts; // EARS::Modules::BuildingStore
			//uint32_t m_NumPeakOwnedVenues = 0;
			//Array<uint32_t> m_CurrentMonopolyUpgrades;
			//Array<EARS::Modules::Family::Decision> m_Decisions; // EARS::Modules::Family::Decision
			//uint32_t m_NumGuards = 0;
			//uint32_t m_NumGuardSlots = 0;
			//float m_GuardTransitTime = 0.0f;
			//float m_MinTurnInterval = 0.0f;
			//float m_MaxTurnInterval = 0.0f;
			//float m_ResponseDelay = 0.0f;
			//float m_TimeSinceLastAction = 0.0f;
			//float m_TimeSinceLastActedUpon = 0.0f;
			//bool m_DesiresToRespond = 0.0f;
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
			char m_FamilyPadding[0x130];
			EARS::Modules::MoneyLedger* m_MoneyLedger = nullptr;
		};
	}
}
