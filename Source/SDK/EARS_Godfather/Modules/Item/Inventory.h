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
		/**
		 * Inventory for both Player and Sentients
		 */
		class Inventory
		{
		public:

			struct Entry
			{
				SafePtr<void*> m_Item; // EARS::Modules::Item
			};

		private:

			void* VTABLE = nullptr;
			Array<EARS::Modules::Inventory::Entry> m_Entries;
		};
	}
}
