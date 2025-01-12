#include "InventoryManager.h"

// Hooks
#include "Addons/Hook.h"

// Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Gun/Gun.h"

// CPP
#include <algorithm>

void EARS::Modules::InventorySlot::SetItemMax(uint32_t InItemMax)
{
	m_ItemMax = InItemMax;

	// Ensure within the new range
	m_ItemCount = std::clamp<uint32_t>(m_ItemCount, 0, m_ItemMax);
}

void EARS::Modules::InventorySlot::SetItemCount(uint32_t InItemCount)
{
	// Ensure input count is within the new range
	m_ItemCount = std::clamp<uint32_t>(InItemCount, 0, m_ItemMax);
}

void EARS::Modules::InventorySlot::IncrementItemCount()
{
	// Ensure within the new range
	m_ItemCount = std::clamp<uint32_t>(++m_ItemCount, 0, m_ItemMax);
}

void EARS::Modules::InventorySlot::DecrementItemCount()
{
	// Ensure within the new range
	m_ItemCount = std::clamp<uint32_t>(--m_ItemCount, 0, m_ItemMax);
}

void EARS::Modules::InventorySlot::SetItem(EARS::Modules::Item* InItem)
{
	// TODO
}

void EARS::Modules::InventoryManager::ToggleUnlimitedAmmo()
{
	//hook::Type<RWS::CEventId> iMsgInfiniteAmmoUnlocked = hook::Type<RWS::CEventId>(0x112AB34);
	//MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, iMsgInfiniteAmmoUnlocked, false);

	SetAllGunsInfiniteAmmo(!m_PlayerHasInfiniteAmmo);
}

void EARS::Modules::InventoryManager::SetItemSlotCount(uint32_t SlotIdx, uint32_t InCount)
{
	if (SlotIdx == Defines::SLOT_BOMB || SlotIdx == Defines::SLOT_DYNAMITE || SlotIdx == Defines::SLOT_MOLOTOV)
	{
		m_Slots[SlotIdx].SetItemCount(InCount);
	}
}

void EARS::Modules::InventoryManager::SetItemSlotMax(uint32_t SlotIdx, uint32_t InMax)
{
	if (SlotIdx == Defines::SLOT_BOMB || SlotIdx == Defines::SLOT_DYNAMITE || SlotIdx == Defines::SLOT_MOLOTOV)
	{
		m_Slots[SlotIdx].SetItemMax(InMax);
	}
}

EARS::Modules::Item* EARS::Modules::InventoryManager::TrySpawnItem(const EARS::Common::guid128_t& ItemID, uint32_t PlayerStream)
{
	return MemUtils::CallClassMethod<EARS::Modules::Item*, EARS::Modules::InventoryManager*, const EARS::Common::guid128_t&, uint32_t>(0x0704B00, this, ItemID, PlayerStream);
}

void EARS::Modules::InventoryManager::AddItemToInventory(EARS::Modules::Item* InItem, bool bShowFanfare)
{
	MemUtils::CallClassMethod<void, EARS::Modules::InventoryManager*, EARS::Modules::Item*, bool>(0x07062A0, this, InItem, bShowFanfare);
}

uint32_t EARS::Modules::InventoryManager::GetItemSlotCount(uint32_t SlotIdx)
{
	return m_Slots[SlotIdx].GetItemCount();
}

uint32_t EARS::Modules::InventoryManager::GetItemSlotMax(uint32_t SlotIdx)
{
	return m_Slots[SlotIdx].GetItemMax();
}

EARS::Modules::Item* EARS::Modules::InventoryManager::GetItemInSlot(uint32_t SlotIdx) const
{
	return m_Slots[SlotIdx].GetItem();
}

void EARS::Modules::InventoryManager::SetAllGunsInfiniteAmmo(bool bInfinite)
{
	m_PlayerHasInfiniteAmmo = bInfinite;

	for (uint32_t i = 0; i < 6; i++)
	{
		if (EARS::Modules::Item* ItemInSlot = m_Slots[i].GetItem())
		{
			if (ItemInSlot->IsGun())
			{
				EARS::Modules::Gun* ItemAsGun = static_cast<EARS::Modules::Gun*>(ItemInSlot);
				ItemAsGun->SetInfiniteAmmo(bInfinite);
				ItemAsGun->AddAmmo(1000);
			}
		}
	}
}
