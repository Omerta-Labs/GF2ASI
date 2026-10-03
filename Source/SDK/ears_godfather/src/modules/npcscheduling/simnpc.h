#pragma once

// SDK Common
#include "ears_common/array.h"
#include "ears_common/bitflags.h"
#include "ears_common/guid.h"
#include "ears_common/safeptr.h"
#include "ears_common/string.h"
#include "ears_common/rwtypes.h"

// SDK Framework
#include "framework/core/base/base.h"

// SDK Godfather
#include "modules/sentient/sentient_constants.h"

namespace EARS
{
	namespace Modules
	{
		class NPCUpgradeComponent;

		/**
		 * Stores the meta information of an NPC.
		 * Primarily for the games monopoly system.
		 */
		class SimNPC : public EARS::Framework::Base
		{
		public:

			/**
			 * Fetch the localised name of the SimNPC.
			 * Once localized, it remains cached in this instance.
			 */
			String* GetName();

			/**
			 * Add this SimNPC to the Players Family and Crew.
			 */
			void SetIsCrewMember(bool bIsCrewMember);

			/**
			 * Is this SimNPC part of the Players Crew and Family?
			 */
			bool GetIsCrewMember() const;

			// Fetch the NPC associated with this SimNPC.
			EARS::Modules::NPC* GetNPC() const { return m_NPC.GetPtr(); }
			EARS::Common::guid128_t GetNPCGuid() const { return m_NPCGuid; }
			EARS::Modules::SentientRank GetRank() const { return m_Rank; }

			// Fetch the Upgrade Component for this SimNPC.
			// After extensive investigation, it was found that this is stored on the SimNPC.
			// This is likely due to the fact that the NPC derived component has monopoly upgrades too
			EARS::Modules::NPCUpgradeComponent* GetUpgradeComponent() const;

		private:

			RwV3d m_CurrentPosition;
			Flags32 m_Flags;
			String m_Name;
			int32_t m_SpawnWhenNearPlayer = 0;
			int32_t m_SpawnIsHighPriority = 0;
			int32_t m_KeepOutOfSpawnPool = 0;
			EARS::Common::guid128_t m_NPCGuid;
			int32_t m_SocialClass = 0;
			int32_t m_ClothingFormality = 0;
			int32_t m_ClimateEnum = 0;
			int32_t m_PaletteEnum = 0;
			SafePtr<EARS::Modules::NPC> m_NPC;
			//SafePtr<void*> m_SubSpawner;
			Array<void*> m_Listeners;
			int32_t m_FamilyCategory = 0;
			String m_DebugName;
			uint32_t m_NameHashID = 0;
			uint32_t m_FamilyID = 0;
			uint32_t m_VenueID = 0;
			EARS::Modules::SentientRank m_Rank = EARS::Modules::SentientRank::RANK_CIVILIAN;
			Flags32 m_NPCFlags;
			uint32_t m_RoleHashID = 0;
			uint32_t m_CityHashID = 0;
		};
	} // Modules
} // EARS

