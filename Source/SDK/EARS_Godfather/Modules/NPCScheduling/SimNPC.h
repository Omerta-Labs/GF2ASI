#pragma once

// SDK Common
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Common/RwMaths.h"

// SDK Framework
#include "SDK/EARS_Framework/Core/Base/Base.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * Stores the meta information of an NPC.
		 * This does not appear to consist of the actual rendered NPC.
		 */
		class SimNPC : public EARS::Framework::Base
		{
		public:

			/**
			 * Fetch the localised name of the SimNPC.
			 * Once localized, it remains cached in this instance.
			 */
			String* GetName();

			// Fetch the NPC associated with this SimNPC.
			EARS::Modules::NPC* GetNPC() const { return m_NPC.GetPtr(); }

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
			uint32_t m_Rank = 0; // EARS::Modules::SentientRank
			Flags32 m_NPCFlags;
			uint32_t m_RoleHashID = 0;
			uint32_t m_CityHashID = 0;
		};
	} // Modules
} // EARS

