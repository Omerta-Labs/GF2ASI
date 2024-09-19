#pragma once

#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"

// SDK
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// CPP
#include <cstdint>

namespace EARS
{
	namespace Modules
	{
		class NPCCrewComponent;
		class NPCUpgradeComponent;

		/**
		 * The NPC class for The Godfather II
		 */
		class NPC : public Sentient
		{
		public:

			// Getters
			EARS::Modules::NPCCrewComponent* GetCrewComponent() const;
			EARS::Modules::NPCUpgradeComponent* GetUpgradeComponent() const;

		private:

		};
	} // Modules
} // EARS
