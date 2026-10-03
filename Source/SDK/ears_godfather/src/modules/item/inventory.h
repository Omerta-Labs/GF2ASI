#pragma once

// Common
#include "SDK/ears_common/include/ears_common/array.h"
#include "SDK/ears_common/include/ears_common/safeptr.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		class Item;

		/**
		 * Inventory for both Player and Sentients
		 */
		class Inventory
		{
		public:

			/**
			 * Fetch the Item stored within an Inventory slot.
			 */
			EARS::Modules::Item* GetItemByIndex(const uint32_t Index);
			EARS::Modules::Item* GetItemByIndex(const uint32_t Index) const;

			// getters
			inline uint32_t CountItems() const { return m_Entries.Size(); }

		private:

			struct Entry
			{
			public:

				// getters
				EARS::Modules::Item* GetItem() const { return m_Item.GetPtr(); }

			private:

				SafePtr<EARS::Modules::Item> m_Item;
			};

			Array<EARS::Modules::Inventory::Entry> m_Entries;
		};
	}
}
