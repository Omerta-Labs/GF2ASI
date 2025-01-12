#include "Item.h"

void EARS::Modules::Item::SetFanFareWhenAcquiredFlag(bool bValue)
{
	if (bValue)
	{
		m_ItemFlags.Set(0x40);
	}
	else
	{
		m_ItemFlags.Clear(0x40);
	}
}

bool EARS::Modules::Item::GetFanFareWhenAcquiredFlag() const
{
	return m_ItemFlags.Test(0x40);
}

void EARS::Modules::Item::SetForceIntoInventoryFlag(bool bValue)
{
	if (bValue)
	{
		m_ListItemFlags |= 1;
	}
	else
	{
		m_ListItemFlags &= 0xFFFE;
	}
}

bool EARS::Modules::Item::GetForceIntoInventoryFlag() const
{
	return (m_ListItemFlags & 1);
}
