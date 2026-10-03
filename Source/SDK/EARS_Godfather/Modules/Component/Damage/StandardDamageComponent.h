#pragma once

// Common
#include "SDK/EARS_Common/Bitflags.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Component/Damage/DamageComponent.h"

namespace EARS
{
	namespace Modules
	{
		class StandardDamageComponent : public DamageComponent
		{
		public:

			void SetInvincible(const bool bIsInvincible);

			bool IsInvicible() const;

		private:

			float m_Health = 0.0f;
			float m_MaxHealth = 0.0f;
			Flags32 m_DamageableFlags;
			int32_t m_InvincibleRefCount = 0;
		};
	} // Modules
} // EARS
