#pragma once

// SDK (Common)
#include "SDK/ears_common/include/ears_common/array.h"
#include "SDK/ears_common/include/ears_common/string.h"

namespace EARS
{
	namespace Modules
	{
		class BuildingStore;

		class MonopolyData
		{
			int z = 0;
		};

		class Monopoly
		{
		public:

			String* GetDisplayName();

			String* GetPerkDescription();

		private:

			struct OwnerShare
			{
				uint32_t m_FamilyID = 0;

				uint32_t m_NumOwned = 0;
			};

			const EARS::Modules::MonopolyData* m_MonopolyData = nullptr;

			Array<EARS::Modules::BuildingStore*> m_Stores;

			Array<OwnerShare> m_Shares;

			String m_DisplayName;

			String m_PerkDescription;
		};
	}
}
