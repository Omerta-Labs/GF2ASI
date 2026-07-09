#pragma once

// SDK Framework
#include "SDK/EARS_Framework/Core/Base/Base.h"

// SDK Common
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/String.h"

// SDK Godfather
#include "SDK/EARS_Godfather/Modules/Families/MadeMan.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/Scoring/MoneyLedger.h"

// C++
#include <functional>

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class BuildingStore;
		class Sentient;
		class MadeMan;

		enum class FamilyCategory : uint32_t
		{
			FamilyCategory_REF = 0xFFFFFFFF,
			FAMILY_CATEGORY_INNOCENT = 0x0,
			FAMILY_CATEGORY_WORKER = 0x1,
			FAMILY_CATEGORY_LAW = 0x2,
			FAMILY_CATEGORY_GANGSTER = 0x3,
			FamilyCategory_MAX_VALUE = 0x4,
		};

		enum class FamilyFlags : uint32_t
		{
			FamilyFlags_REF = 0xFFFFFFFF,
			FAMILY_IS_HIDDEN_IN_UI = 0x1,
			FAMILY_IS_FRIENDLY_FIRE_OFF = 0x2,
			FAMILY_USES_STIMULUS_SOURCE_ENEMIES = 0x4,
			FAMILY_CAN_HAVE_CRIMES_REPORTED = 0x8,
			FAMILY_HAS_BEEN_ELIMINATED = 0x10,
			FAMILY_IS_HIDDEN_FROM_PLAYER = 0x20,
			FAMILY_NO_STRATEGY_ACTIONS = 0x40,
			FAMILY_NEW_HIT_INTEL_REVEALED = 0x80,
			FAMILY_DO_NOT_PLAY_REPUTATION_CHATTER = 0x100,
			FAMILY_COMPOUND_UNLOCKED = 0x200,
			FAMILY_SHOW_DON_AS_ALIVE = 0x400,
			FAMILY_SHOW_DON_AS_DEAD = 0x800,
			FAMILY_CREW_COMMAND_ENEMY = 0x1000,
		};

		/**
		 * An instance of a Family for Godfather II
		 */
		class Family : public EARS::Framework::Base
		{
		public:

			struct OmertaEntry
			{
				uint32_t m_FamilyID = 0;
				float m_Omerta = 0.0f;
			};

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

			/**
			 * Get the Ally ID from a specific index, if available.
			 * Note that this will cause a crash if Index is beyond current Ally count.
			 * @param Index - The Index to use when accessing Ally Family array (max 3).
			 * @return uint32_t - The Ally Family ID.
			 */
			uint32_t GetAllyFamilyID(const uint32_t Index) const;

			/**
			 * Request that all members of this family are fully healed and out of Hospital
			 * @return bool - Whether or not we have made changes (eg. if any member has their health replenished)
			 */
			bool QuickRecovery() const;

			/**
			 * Request that a specific member of the family is fully recovered (taken out of hospital)
			 * @return InMadeMan - The Made Man we would like to heal back to 100% health
			 * @return bool - Whether or not it achieved the desired effect.
			 */
			bool QuickRecovery(EARS::Modules::MadeMan& InMadeMan) const;

			/**
			 * Request that all members of this family are broken out of Jail.
			 * @return bool - Whether or not we have made changes (eg. if any member has been taken out of Jail)
			 */
			bool Jailbreak() const;

			/**
			 * Request that a specific member of the family is broken out of Jail.
			 * @return InMadeMan - The Made Man we would like to break out of Jail
			 * @return bool - Whether or not it achieved the desired effect.
			 */
			bool Jailbreak(EARS::Modules::MadeMan& InMadeMan) const;

			/**
			 * Request that a specific Made Man is hopitalised, using their SimNPC.
			 * Internally the Family will be used to find their associated Made Man.
			 * @return InSimNPC - The SimNPC used to find the Made Man to hospitalise
			 */
			void HospitalizeMadeMan(EARS::Modules::SimNPC& InSimNPC) const;

			/**
			 * Request that a specific Made Man is sent to jail, using their SimNPC.
			 * Internally the Family will be used to find their associated Made Man.
			 * @return InSimNPC - The SimNPC used to find the Made Man send to jail
			 */
			void IncarcerateMadeMan(EARS::Modules::SimNPC& InSimNPC) const;

			/**
			 * Request that a specific Made Man is killed off, using their SimNPC
			 * Internally the Family will be used to find their associated Made Man.
			 * @return InSimNPC - The SimNPC used to find the Made Man and kill
			 */
			void KillMadeMan(EARS::Modules::SimNPC& InSimNPC) const;

			/**
			 * Revive a Made Man if eliminated.
			 * They'll travel back to their Family compound.
			 * NB: *NOT PART OF ORIGINAL GAME*
			 */
			void ReviveMadeMan(EARS::Modules::SimNPC& InSimNPC) const;

			/**
			 * Revive a Family if eliminated. 
			 * Will revive all Made Men and give back their compound.
			 * They'll also enter the strategy game.
			 * NB: *NOT PART OF ORIGINAL GAME*
			 */
			void ReviveFamily();
			
			/**
			 * Get a Made Man from a specific slot in the list
			 * @param Index - The index we will use to access the Made Man
			 * @return MadeMan - The Made Man in the given slot
			 */
			EARS::Modules::MadeMan* GetMadeManByIndex(const uint32_t Index) const;

			typedef std::function<void(OmertaEntry&)> TVisitOmertaEntryFunctor;
			void ForEachOmertaTable(const TVisitOmertaEntryFunctor& InFunction);

			// getters
			uint32_t GetFamilyID() const { return m_FamilyID; }
			uint32_t GetNumAllies() const { return m_NumAllyFamilies; }
			uint32_t GetNumMadeMen() const { return m_MadeMen.Size(); }
			const String* GetInternalName() const;
			const String* GetSingularName() const;
			const String* GetPluralName() const;
			float GetMinTurnInterval() const { return m_MinTurnInterval; }
			float GetMaxTurnInterval() const { return m_MaxTurnInterval; }
			float GetResponseDelay() const { return m_ResponseDelay; }
			uint32_t GetCompoundVenueID() const { return m_CompoundVenueID; }
			bool HasBeenEliminated() const { return m_Flags.Test((int)FamilyFlags::FAMILY_HAS_BEEN_ELIMINATED); }

			// made men API
			float GetMadeManHospitalTime() const { return m_HospitalTime; }
			float GetMadeManJailTime() const { return m_JailTime; }

			// simple setters
			void SetMinTurnInterval(const float InValue) { m_MinTurnInterval = InValue; }
			void SetMaxTurnInterval(const float InValue) { m_MaxTurnInterval = InValue; }
			void SetResponseDelay(const float InValue) { m_ResponseDelay = InValue; }

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
				void* m_VenueOfInterest = nullptr; // EARS::Modules::BuildingStore
				bool m_bIsPlayerInferred = false;
				Array<EARS::Modules::SimNPC*> m_RespondersToUse;
			};

			/**
			 * Find Made Man using SimNPC. Output is the index in Made Man array.
			 * @param InSimNPC - The NPC we want to find in the Made Man array.
			 * @return uint32_t - If found, a valid index in the array. If not, -1.
			 */
			int32_t FindMadeManIndex(const EARS::Modules::SimNPC& InSimNPC) const;

			void SetMadeManState(EARS::Modules::SimNPC& InSimNPC, EARS::Modules::MadeManState NewState, uint32_t VenueID, float Countdown) const;
			void SetMadeManState(EARS::Modules::MadeMan& InMadeMan, EARS::Modules::MadeManState NewState, uint32_t VenueID, float Countdown) const;

			EARS::Modules::FamilyCategory m_Category = FamilyCategory::FamilyCategory_REF;				// 0x50
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
			float m_Balance = 0;																		// 0x100 - 0x104
			char m_FamilyPadding_1[0x4]; //float m_Income = 0; ??
			Array<EARS::Modules::BuildingStore*> m_OwnedRackets;
			Array<EARS::Modules::BuildingStore*> m_OwnedFronts;											// 0x114 - 0x120
			//uint32_t m_NumPeakOwnedVenues = 0;
			//Array<uint32_t> m_CurrentMonopolyUpgrades;
			char m_FamilyPAdding_2[0x10];
			Array<EARS::Modules::Family::Decision> m_Decisions;											// 0x130 - 13C
			//uint32_t m_NumGuards = 0;
			//uint32_t m_NumGuardSlots = 0;
			//float m_GuardTransitTime = 0.0f;
			char m_FamilyPadding_3[0xC];
			float m_MinTurnInterval = 0.0f;
			float m_MaxTurnInterval = 0.0f;
			float m_ResponseDelay = 0.0f;																// 0x150 - 0x154
			char m_FamilyPadding_4[0x38];
			//float m_TimeSinceLastAction = 0.0f;
			//float m_TimeSinceLastActedUpon = 0.0f;
			//bool m_DesiresToRespond = 0.0f;
			//EARS::Modules::Family::Bonuses m_bonuses;
			Array<EARS::Modules::Family::OmertaEntry> m_OmertaTable;									// 0x18C - 0x198
			char m_FamilyPadding_5[4]; //float m_MaxOmerta;
			void* m_VenueConsidering = nullptr;		// EARS::Modules::BuildingStore						// 0x19C - 0x1A0													
			char m_FamilyPadding_6[4]; //EARS::Modules::Family* m_pFamilyConsidering;
			Array<EARS::Modules::MadeMan*> m_MadeMen;													// 0x1A4 - 0x1B0
			char m_FamilyPadding_7[0x20];
			//float m_aRankPowerMappings[8];
			void* m_Data = nullptr;																		// 0x1D0 - 0x1D4
			float m_HospitalTime = 0.0f;																// 0x1D4 - 0x1D8
			float m_JailTime = 0.0f;																	// 0x1D8 - 0x1DC
			float m_CooldownTime = 0.0f;																// 0x1E0 - 0x1E4
			float m_HangoutTime = 0.0f;																	// 0x1E4 - 0x1E8
			float m_VenueBombingDelay = 0.0f;															// 0x1E8 - 0x1EC
			char m_FamilyPadding_8[0x44];
			//RWS::CEventId m_performStingMsg;
			//RWS::CEventId m_jailbreakMsg;
			//RWS::CEventId m_quickRecoveryMsg;
			//RWS::CEventId m_unionHelpMsg;
			//RWS::CEventId m_lostAllVenuesMsg;
			//RWS::CEventId m_eliminateMsg;
			//RWS::CEventId m_gainedMonopolyMsg;
			//RWS::CEventId m_lostMonopolyMsg;
			uint32_t m_CompoundVenueID = 0;																// 0x22C
			EARS::Modules::MoneyLedger* m_MoneyLedger = nullptr;										// 0x230
		};

		static_assert(sizeof(EARS::Modules::Family) == 0x234, "Expected EARS::Modules::Family to have a size of 0x234");
	}
}
