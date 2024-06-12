#pragma once

// SDK
#include "SDK/EARS_Godfather/Modules/Components/UpgradeComponent.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * An Upgrade component primarily used by the Player class
		 */
		class PlayerUpgradeComponent : public EARS::Modules::UpgradeComponent
		{
		public:

			// Fetch the index given to this component at runtime
			static uint32_t GetComponentIndex();

		private:
		};
	}
}
