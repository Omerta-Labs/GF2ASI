#include "NPC.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Components/NPCUpgradeComponent.h"
#include "SDK/EARS_Godfather/Modules/Components/NPC/NPCCrewComponent.h"

// Hook
#include <Addons/Hook.h>

EARS::Modules::NPCCrewComponent* EARS::Modules::NPC::GetCrewComponent() const
{
	hook::Type<uint32_t> CrewComponentIndex = hook::Type<uint32_t>(0x1131040);
	EARS::Framework::Component* FoundComp = MemUtils::CallClassMethod<EARS::Framework::Component*, const EARS::Modules::NPC*, uint32_t>(0x043B870, this, CrewComponentIndex);
	return (EARS::Modules::NPCCrewComponent*)FoundComp;
}

EARS::Modules::NPCUpgradeComponent* EARS::Modules::NPC::GetUpgradeComponent() const
{
	EARS::Framework::Component* FoundComp = MemUtils::CallClassMethod<EARS::Framework::Component*, const EARS::Modules::NPC*, uint32_t>(0x043B870, this, EARS::Modules::NPCUpgradeComponent::GetComponentIndex());
	return (EARS::Modules::NPCUpgradeComponent*)FoundComp;
}
