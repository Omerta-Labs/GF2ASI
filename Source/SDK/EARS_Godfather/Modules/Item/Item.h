#pragma once

// Common
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/String.h"

// Framework
#include "SDK/EARS_Framework/Core/Entity/Entity.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		class Item : public EARS::Framework::Entity
		{
		public:

			virtual ~Item();

			enum Categories : __int32
			{
				CATEGORY_UNDEF = 0x0,
				CATEGORY_GUN = 0x1,
				CATEGORY_COMBAT = 0x2,
				CATEGORY_PROP = 0x3,
				CATEGORY_MODIFIER = 0x4,
				CATEGORY_THROWABLE = 0x5,
				CATEGORY_NONGUNDISARMABLE = 0x6,
				CATEGORY_MAX = 0x7,
			};

			enum CombatAnimType : __int32
			{
				COMBATANIMTYPE_UNARMED = 0,
				COMBATANIMTYPE_1H_FIREARM = 1,
				COMBATANIMTYPE_2H_FIREARM = 2,
				COMBATANIMTYPE_1H_MELEE = 3,
				COMBATANIMTYPE_2H_MELEE = 4,
				COMBATANIMTYPE_VEHICLE = 5,
				COMBATANIMTYPE_NUMTYPES = 6
			};

			enum Handedness : __int32
			{
				NO_HANDED = 0,
				ONE_HANDED = 1,
				TWO_HANDED = 2
			};

			enum ItemFlags : __int32
			{
				FLAG_PICKUPABLE = 2,
				FLAG_THROWABLE = 4,
				FLAG_MELEEABLE = 8,
				FLAG_IS_INVENTORY = 16,
				FLAG_BREAKABLE = 32,
				FLAG_NO_FANFARE = 64,
				FLAG_DROPPED = 128,
				FLAG_DROPPING = 256,
				FLAG_USING_HOLD_ITEM_VFX = 512,
				FLAG_JUST_DROPPED = 1024,
				FLAG_USE_PHANTOM = 2048,
				FLAG_HID_UNHELD_VFX = 4096,
				FLAG_ADDED_TO_WORLD = 8192,
				FLAG_CHASE_ITEM = 16384,
				FLAG_CLEAN_FROM_INVENTORY = 32768
			};

			enum ItemLength : __int32
			{
				LENGTH_SHORT = 0,
				LENGTH_LONG = 1
			};

			// query specific info
			inline bool IsCombatItem() const { return m_ItemType == Categories::CATEGORY_COMBAT; }
			inline bool IsGun() const { return m_ItemCategory == Categories::CATEGORY_GUN; }
			// EARS::Modules::Item::IsInventoryItem
			// EARS::Modules::Item::IsMeleeable
			inline bool IsNonGunDisarmable() const { return m_ItemCategory == Categories::CATEGORY_NONGUNDISARMABLE; }
			inline bool IsOneHanded() const { return m_ItemHandUse == Handedness::ONE_HANDED; }
			inline bool IsProp() const { return m_ItemCategory == Categories::CATEGORY_PROP; }
			// EARS::Modules::Item::IsThrowable
			// EARS::Modules::Item::IsTouched
			inline bool IsTwoHanded() const { return m_ItemHandUse == Handedness::TWO_HANDED; }

			// Manage FanFare flag on the Item
			void SetFanFareWhenAcquiredFlag(bool bValue);
			bool GetFanFareWhenAcquiredFlag() const;

			// Manage the 'Force into inventory' flag on the Item
			void SetForceIntoInventoryFlag(bool bValue);
			bool GetForceIntoInventoryFlag() const;

			// getters
			inline uint8_t GetCombatAnimType() const { return m_CombatAnimType; }
			inline uint8_t GetItemCategory() const { return m_ItemCategory; }
			inline uint8_t GetItemHandedness() const { return m_ItemHandUse; }
			inline uint8_t GetItemType() const { return m_ItemType; }
			inline uint8_t GetMeleeItemLength() const { return m_MeleeItemLength; }
			inline uint8_t GetSafeHouseIndex() const { return m_SafeHouseIndex; }
			inline const char* GetName() const { return m_Name.c_str(); }
			inline uint32_t GetNameHash() const { return m_HashedName; }

		private:

			char m_Item_Padding[0x38];
			uint8_t m_ItemCategory = 0;		// 0x1A8
			uint8_t m_CombatAnimType = 0;
			uint8_t m_ItemHandUse = 0;
			uint8_t m_MeleeItemLength = 0;
			uint8_t m_SafeHouseIndex = 0;
			uint8_t m_ItemType = 0;
			void* m_RigidBody = nullptr;
			void* m_HavokResource = nullptr;
			String m_Name;
			uint32_t m_HashedName = 0;
			float m_FadeTime = 0.0f;
			Flags32 m_ItemFlags;
			uint16_t m_ListItemFlags = 0;
		};
	} // Modules
} // EARS
