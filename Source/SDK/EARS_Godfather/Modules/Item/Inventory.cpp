#include "Inventory.h"

EARS::Modules::Item* EARS::Modules::Inventory::GetItemByIndex(const uint32_t Index)
{
	return m_Entries[Index].GetItem();
}

EARS::Modules::Item* EARS::Modules::Inventory::GetItemByIndex(const uint32_t Index) const
{
	return m_Entries[Index].GetItem();
}
