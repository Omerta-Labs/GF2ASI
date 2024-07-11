#pragma once

// SDK Common
#include "SDK/EARS_Common/Bitflags.h"

// SDK Godfather
#include "SDK/EARS_Godfather/Modules/Item/Item.h"

namespace EARS
{
	namespace Modules
	{
		/**
		 * Firearm class in Godfather II
		 */
		class Gun : public Item
		{
		public:

			enum GunFlags : uint32_t
			{
				FLAG_AUTOMATIC = 0x1,
				FLAG_DONOTMERGE = 0x2,
				FLAG_PLAYEROWNED = 0x4,
				FLAG_FULLYINITIALIZED = 0x8,
				FLAG_FIREDSINCEDRAWN = 0x10,
				FLAG_INFINITE_AMMO = 0x20,
				FLAG_PLAYER_HAS_INFINITE_AMMO = 0x40,
				FLAG_RETICULE_MAX_EXPANDED = 0x80,
				FLAG_BULLET_IMPACT_SILENT = 0x100,
			};

			/**
			 * Update the 'Infinite Ammo' flag for this weapon.
			 * Ammo becomes infinite, in a sense that clips are still expendable
			 * but the total ammo count always remains the same.
			 */
			void SetInfiniteAmmo(const bool bInfinite);

			/**
			 * Add Ammo to the AmmoStore. If Infinite Ammo is not active,
			 * Then the Ammo is always capped to the limits defined in data
			 */
			void AddAmmo(const uint32_t Ammo);

		private:

			char m_Padding_Gun0[0x10C];
			Flags32 m_GunFlags;
		};
	} // Modules
} // EARS
