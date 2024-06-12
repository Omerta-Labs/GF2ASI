#pragma once

// SDK
#include "SDK/EARS_Godfather/Modules/Components/UpgradeComponent.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * A component which provides the ability to associates upgrades
		 * to a given entity.
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
