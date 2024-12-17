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
		 * Handles Player and Player Family Mobface
		 * TODO: Finish off base classes
		 */
		class MobfaceManager : public RWS::CEventHandler
		{
		public:

			/* Build model from the saved data in the save game file */
			void BuildModelFromSavedData(const PlayerFamilyTree::FamilyTreeSlot SlotIndex, EARS::Modules::PartedAnimated* TargetModel);

			/* get the game instance of the MobfaceManager */
			static MobfaceManager* GetInstance();

		private:

		};
	} // Modules
} // EARS
