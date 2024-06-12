#include "UpgradeComponent.h"

// Addons
#include "Addons/Hook.h"

void EARS::Modules::UpgradeComponent::ModifyAllUpgradeParts(bool bShowParts)
{
	MemUtils::CallClassMethod<void, EARS::Modules::UpgradeComponent*, bool>(0x09ADD60, this, bShowParts);
}

uint32_t EARS::Modules::UpgradeComponent::GetComponentIndex()
{
	hook::Type<uint32_t> ComponentIndex = hook::Type<uint32_t>(0x01131010);
	return ComponentIndex.get();
}
