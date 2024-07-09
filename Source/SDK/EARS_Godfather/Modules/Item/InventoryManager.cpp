#include "InventoryManager.h"

// Hooks
#include "Addons/Hook.h"

// Framework
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

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

void EARS::Modules::InventoryManager::GiveUnlimitedAmmo()
{
	hook::Type<RWS::CEventId> iMsgInfiniteAmmoUnlocked = hook::Type<RWS::CEventId>(0x112AB34);
	MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, iMsgInfiniteAmmoUnlocked, false);
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

uint32_t EARS::Modules::InventoryManager::GetItemSlotCount(uint32_t SlotIdx)
{
	return m_Slots[SlotIdx].GetItemCount();
}

uint32_t EARS::Modules::InventoryManager::GetItemSlotMax(uint32_t SlotIdx)
{
	return m_Slots[SlotIdx].GetItemMax();
}
