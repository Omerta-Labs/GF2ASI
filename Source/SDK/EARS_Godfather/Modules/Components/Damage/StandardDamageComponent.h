#pragma once

#include "SDK/EARS_Godfather/Modules/Components/Damage/DamageComponent.h"

namespace EARS
{
	namespace Modules
	{
		class StandardDamageComponent : public DamageComponent
		{
		public:

			void SetInvincible(const bool bIsInvincible);
		};
	} // Modules
} // EARS
