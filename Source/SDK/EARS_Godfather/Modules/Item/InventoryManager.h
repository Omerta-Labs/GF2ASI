#pragma once

// Common
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/SafePtr.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		class Item;
		class Inventory;
		class Player;

		/**
		 * Individual Slot for a Weapon in the Players Inventory.
		 */
		class InventorySlot
		{
		public:

			/**
			 * Update current number of this item in the slot.
			 * Automatically clamps between 0 and m_ItemMax
			 */
			void SetItemCount(uint32_t InItemCount);

			/**
			 * Update maximum number of this item in the slot.
			 * Automatically clamps between 0 and m_ItemMax
			 */
			void SetItemMax(uint32_t InItemMax);

			/**
			 * Increment the current count by 1.
			 * Automatically clamps between 0 and m_ItemMax
			 */
			void IncrementItemCount();

			/**
			 * Decrement the current count by 1.
			 * Automatically clamps between 0 and m_ItemMax
			 */
			void DecrementItemCount();

			/**
			 * Assign a new item to the Inventory Slot.
			 */
			void SetItem(EARS::Modules::Item* InItem);

			// getters
			inline uint32_t GetItemCount() const { return m_ItemCount; }
			inline uint32_t GetItemMax() const { return m_ItemMax; }
			inline bool IsSlotFull() const { return m_ItemCount >= m_ItemMax; }
			inline EARS::Modules::Item* GetItem() const { return m_Item.GetPtr(); }

		private:

			void* VTABLE = nullptr;
			SafePtr<EARS::Modules::Item> m_Item; // EARS::Modules::Item
			uint32_t m_ItemCount = 0;
			uint32_t m_ItemMax = 0;
		};

		/**
		 * Inventory Manager, primarily for the Player.
		 * Wraps weapon slots and inventory for the Player.
		 * Also includes stuff for swapping weapons.
		 */
		class InventoryManager /* RWS::CEventHandler, IPersistable */
		{
		public:

			enum Defines : int32_t
			{
				SLOT_RIFLE = 0x0,
				SLOT_PISTOL = 0x1,
				SLOT_MAGNUM = 0x2,
				SLOT_TOMMYGUN = 0x3,
				SLOT_SHOTGUN = 0x4,
				SLOT_MELEE = 0x5,
				SLOT_MOLOTOV = 0x6,
				SLOT_DYNAMITE = 0x7,
				SLOT_BOMB = 0x8,
				SLOT_STRANGLER = 0x9,
				SLOT_FISTMODIFIER = 0xA,
				SLOT_COUNT = 0xB,
				SLOT_GUN_FIRST = 0x0,
				SLOT_GUN_LAST = 0x4,
				SLOT_GUN_COUNT = 0x5,
				WEAPONSLOT_EQUIPPABLE_START = 0x0,
				WEAPONSLOT_EQUIPPABLE_END = 0xA,
			};

			// Not Part of GF2
			void GiveUnlimitedAmmo();

			/**
			 * Modify the current slot count for a specific index.
			 * NB: Only SLOT_BOMB, SLOT_MOLOTOV and SLOT_DYNAMITE is supported!
			 */
			void SetItemSlotCount(uint32_t SlotIdx, uint32_t InCount);

			/**
			 * Modify the maximum slot count for a specific index.
			 * NB: Only SLOT_BOMB, SLOT_MOLOTOV and SLOT_DYNAMITE is supported!
			 */
			void SetItemSlotMax(uint32_t SlotIdx, uint32_t InMax);

			// Getters for a specific slot. All types are supported
			uint32_t GetItemSlotCount(uint32_t SlotIdx);
			uint32_t GetItemSlotMax(uint32_t SlotIdx);
			EARS::Modules::Item* GetItemInSlot(uint32_t SlotIdx) const;

		private:

			char m_Padding[0x10]; // (EventHandler and Persistable)
			EARS::Modules::Inventory* m_Inventory = nullptr;
			EARS::Modules::InventorySlot m_Slots[Defines::SLOT_COUNT];
			EARS::Modules::Player* m_Player = nullptr;
			bool m_bPlayerHasInfiniteAmmo = false;
			uint32_t m_CurrentSlot = 0; // Maybe
			bool m_bWaitingOnInput = false;
			SafePtr<EARS::Modules::Item> m_OldItem;
			SafePtr<EARS::Modules::Item> m_NewItem;

			// TODO: This is the remaining part of InventoryManager
			// Need to figure out whether it matches the memory layout

			//EARS::Modules::InventorySlot* m_pSwapSlot;
			//unsigned int m_slotsExcludedFromRotationMask;
			//bool m_AlreadyPickedUpItemsArray[35];
			//unsigned int m_savedInventorySize;
			//char* m_savedInventoryBuffer;
			//bool m_bInventoryBuffer;
			//bool m_bHasRegisteredPersistable;

		};
	}
}
