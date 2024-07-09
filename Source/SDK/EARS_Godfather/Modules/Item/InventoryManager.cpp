#include "InventoryManager.h"

// Hooks
#include "Addons/Hook.h"

// Framework
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

void EARS::Modules::InventoryManager::GiveUnlimitedAmmo()
{
	hook::Type<RWS::CEventId> iMsgInfiniteAmmoUnlocked = hook::Type<RWS::CEventId>(0x112AB34);
	MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, iMsgInfiniteAmmoUnlocked, false);
}
