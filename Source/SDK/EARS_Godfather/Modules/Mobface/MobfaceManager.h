#pragma once

// EARS Godfather
#include "SDK/EARS_Godfather/Modules/FamilyTree/PlayerFamilyTree.h"

// EARS Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS
{
	namespace Modules
	{
		// forward declare
		class PartedAnimated;

		/**
		 * Stores state as to whether or not an NPC has stored state in save file
		 * For m_EditedInMobFace and m_EditedInApparel, each bit is an NPC.
		 */
		struct MobFaceSaveHdr
		{
		public:

			uint32_t m_Version = 0;
			uint32_t m_EditedInMobFace = 0;
			uint32_t m_EditedInApparel = 0;
		};

		/**
		 * Handles Player and Player Family Mobface
		 * TODO: Finish off base classes
		 */
		class MobfaceManager : public RWS::CEventHandler
		{
		public:

			/**
			 * Check whether or not the manager stores any state for the specified slot index
			 * This is part of engine code
			 * @param SlotIndex - The Family Tree Slot we want to use when querying the Manager
			 * @return bool - Whether or not we have saved state
			 */
			bool HasSavedData(const PlayerFamilyTree::FamilyTreeSlot SlotIndex) const;

			/* Build model from the saved data in the save game file */
			void BuildModelFromSavedData(const PlayerFamilyTree::FamilyTreeSlot SlotIndex, EARS::Modules::PartedAnimated* TargetModel);

			/* get the game instance of the MobfaceManager */
			static MobfaceManager* GetInstance();

		private:

			bool HasMobFaceSaveData(const uint32_t BitIndex) const;
			bool HasApparelSaveData(const uint32_t BitIndex) const;

			// 0x40
			char m_Mobface_Padding0[0x34];
			EARS::Modules::MobFaceSaveHdr m_SaveHeader;

		};
	} // Modules
} // EARS
