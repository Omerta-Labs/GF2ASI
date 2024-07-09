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
