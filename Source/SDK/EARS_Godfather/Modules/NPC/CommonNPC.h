#pragma once

#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"

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
		 * The CommonNPC class for The Godfather II
		 */
		class CommonNPC : public NPC
		{
		public:

		private:

			char m_Padding_CommonNPC[0x850];

		};

		static_assert(sizeof(CommonNPC) == 0x29D0, "EARS::Modules::CommonNPC must equal 0x29D0");
	} // Modules
} // EARS
