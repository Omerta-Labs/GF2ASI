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
		/**
		 * Rank for Sentient types
		 */
		enum class SentientRank : int32_t
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

		enum class Specialties : int32_t
		{
			SPECIALITY_DEMO = 4,
			SPECIALITY_ARSONIST = 8,
			SPECIALITY_SAFECRACKER = 0x10,
			SPECIALITY_ENGINEER = 0x20,
			SPECIALITY_MEDIC = 0x40,
			SPECIALITY_BRUTE = 0x80
		};

		/**
		 * Stores the meta information of an NPC.
		 * This does not appear to consist of the actual rendered NPC.
		 */
		class SimNPC
		{

		};

		/**
		 * A structure for a Family member.
		 */
		struct PlayerFamilyMember
		{
			enum class Flags : uint32_t
			{
				FLAG_FAMILYMEMBER_SLOT_UNLOCKED = 0x1,
				FLAG_FAMILYMEMBER_SLOT_FILLED = 0x2,
				FLAG_FAMILYMEMBER_SELECTED_FOR_ACTION = 0x4,
			};

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