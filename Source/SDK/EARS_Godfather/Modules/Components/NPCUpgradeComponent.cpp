#include "NPCUpgradeComponent.h"

// Addons
#include "Addons/Hook.h"

uint32_t EARS::Modules::NPCUpgradeComponent::GetComponentIndex()
{
	hook::Type<uint32_t> ComponentIndex = hook::Type<uint32_t>(0x1130FA8);
	return ComponentIndex.get();
}
