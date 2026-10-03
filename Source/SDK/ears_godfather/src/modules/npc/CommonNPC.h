#pragma once

#include "SDK/ears_godfather/src/modules/npc/NPC.h"

// SDK
#include "SDK/ears_common/include/ears_common/Guid.h"
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/CEventHandler.h"

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
