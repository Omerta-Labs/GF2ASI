#include "playerupgradecomponent.h"

// Addons
#include "Platform/MemUtils.h"

uint32_t EARS::Modules::PlayerUpgradeComponent::GetComponentIndex()
{
	hook::Type<uint32_t> ComponentIndex = hook::Type<uint32_t>(0x1131008);
	return ComponentIndex.get();
}
