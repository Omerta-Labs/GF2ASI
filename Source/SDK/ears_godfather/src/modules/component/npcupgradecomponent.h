#pragma once

// SDK
#include "SDK/ears_godfather/src/modules/component/upgradecomponent.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 *  An Upgrade component primarily used by the NPC class
		 */
		class NPCUpgradeComponent : public EARS::Modules::UpgradeComponent
		{
		public:

			// Fetch the index given to this component at runtime
			static uint32_t GetComponentIndex();

		private:
		};
	}
}
