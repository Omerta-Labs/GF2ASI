#pragma once

// SDK
#include "SDK/EARS_Common/SingleLinkedListNodeMixin.h"

namespace EARS
{
	namespace WeaponUtils
	{
		/**
		 * Interface to listen for specific damage events to a specific Entity
		 */
		class IDamageListener : public EARS::Common::SingleLinkedListNodeMixin<IDamageListener>
		{
		public:

			virtual void OnDamage() = 0;

			virtual void OnReceivingDeathDamage() = 0;

			virtual void OnDeath() = 0;

			virtual void OnRegenHealth() = 0;

			virtual void OnDelete() = 0;

		private:

			uint16_t m_LinkPriority = 0;
		};

		/**
		 * Interface to listen for health changes for a specific Entity
		 */
		class IHealthListener : public EARS::Common::SingleLinkedListNodeMixin<IHealthListener>
		{
		public:

			virtual void OnHealthChange() = 0;

			virtual void OnMaxHealthChange() = 0;
		};
	}
}