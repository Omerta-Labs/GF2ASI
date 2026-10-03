#pragma once

#include "modules/npc/npc.h"

// SDK
#include "ears_common/guid.h"
#include "framework/core/eventhandler/ceventhandler.h"

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
