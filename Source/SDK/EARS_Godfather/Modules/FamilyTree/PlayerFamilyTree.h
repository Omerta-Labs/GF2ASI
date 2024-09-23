#pragma once

// SDK
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/SafePtr.h"

// CPP
#include <functional>
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		class SimNPC;

		/**
		 * Rank for Sentient types
		 */
		enum class SentientRank : uint32_t
		{
			SentientRank_REF = 0xFFFFFFFF,
			RANK_CIVILIAN = 0x0,
			RANK_ASSOCIATE = 0x1,
			RANK_ASSOCIATE_STRONG = 0x2,
			RANK_SOLDIER = 0x3,
			RANK_CAPO = 0x4,
			RANK_UNDERBOSS = 0x5,
			RANK_CONSIGLIERE = 0x6,
			RANK_DON = 0x7,
			SentientRank_MAX_VALUE = 0x8,
		};

		enum class Specialties : uint32_t
		{
			SPECIALITY_DEMO = 2,			// 0x4
			SPECIALITY_ARSONIST = 3,		// 0x8
			SPECIALITY_SAFECRACKER = 4,		// 0x10
			SPECIALITY_ENGINEER = 5,		// 0x20
			SPECIALITY_MEDIC = 6,			// 0x40
			SPECIALITY_BRUISER = 7			// 0x80
		};

		/**
		 * A structure for a Family member.
		 */
		struct PlayerFamilyMember
		{
		public:

			enum class Flags : uint32_t
			{
				FLAG_FAMILYMEMBER_SLOT_UNLOCKED = 0x1,
				FLAG_FAMILYMEMBER_SLOT_FILLED = 0x2,
				FLAG_FAMILYMEMBER_SELECTED_FOR_ACTION = 0x4,
			};

			// Specialty API.
			bool HasSpecialty(const EARS::Modules::Specialties NewSpeciality) const;

			// Add a Specialty to the Family Member.
			// Should automatically update the hud if the SimNPC is present
			// (TODO: Ensure that the HUD and crew system is updated to reflect addition)
			void AddSpecialty(const EARS::Modules::Specialties NewSpeciality);

			// Toggle a specific Specialty to the Family Member.
			// Should automatically update the hud if the SimNPC is present
			// (TODO: Ensure that the HUD and crew system is updated to reflect addition)
			void ToggleSpecialty(const EARS::Modules::Specialties NewSpeciality);

			// Add a Specialty from the Family Member.
			// Should automatically update the hud if the SimNPC is present
			// (TODO: Ensure that the HUD and crew system is updated to reflect addition)
			void RemoveSpecialty(const EARS::Modules::Specialties Speciality);

			void JoinCrew();

			void LeaveCrew();

			// Getters
			Flags32 GetFlags() const { return m_Flags; }
			EARS::Modules::SentientRank GetRank() const { return m_Rank; }
			EARS::Modules::SimNPC* GetSimNPC() const { return m_SimNPC.GetPtr(); }
			EARS::Common::guid128_t GetWeaponGUID() const { return m_WeaponGUID; }

		private:

			// React to this Family Member's Specialties updating
			void OnSpecialitiesUpdated();

			Flags32 m_Flags;
			EARS::Modules::SentientRank m_Rank = SentientRank::SentientRank_REF;
			SafePtr<EARS::Modules::SimNPC> m_SimNPC;
			uint32_t m_Specialties = 0;
			EARS::Common::guid128_t m_WeaponGUID;
		};

		/**
		 * Stores information of the Family Tree, which type currently used and its members
		 */
		class PlayerFamilyTree
		{
		public:

			/**
			 * Utility function to iterate through all loaded Cities
			 * All const, we do not expect to modify any of the assemblies during iteration.
			 * All headers pass-by-ref, so none of them should be nullptr.
			 * Function does not exist in GF2 exe, merely utility header for us to use.
			 */
			typedef std::function<void(EARS::Modules::PlayerFamilyMember&)> TVisitFamilyMemberFunctor;
			void ForEachMember(const TVisitFamilyMemberFunctor& InFunction);

			/** Access the CorleoneFamilyData instance */
			static PlayerFamilyTree* GetInstance();

		private:

			char m_Padding_0[0x14];
			uint32_t m_CurrentTreeType = 0;
			EARS::Modules::PlayerFamilyMember m_FamilyMembers[7];
		};
	} // Framework
} // EARS