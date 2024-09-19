#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Component/Component.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * A component which provides the ability to associates upgrades
		 * to a given entity.
		 */
		class UpgradeComponent : public EARS::Framework::Component
		{
		public:

			/**
			 * Updates the visiblity of all upgrade parts
			 * currently active.
			 * (NB: Part of base engine)
			 */
			void ModifyAllUpgradeParts(bool bShowParts);

			// Fetch the index given to this component at runtime
			static uint32_t GetComponentIndex();

		private:
		};
	}
}
