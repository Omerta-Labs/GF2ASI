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

		/**
		 * Inventory for both Player and Sentients
		 */
		class Inventory
		{
		public:

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
