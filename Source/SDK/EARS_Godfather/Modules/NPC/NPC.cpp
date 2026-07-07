#include "NPC.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Components/NPC/NPCCrewComponent.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"

// Hook
#include <Addons/Hook.h>

void EARS::Modules::NPC::ActivateHUDIndicator(uint32_t NewIndicatorType, const char* InIndicatorVfxName)
{
	MemUtils::CallClassMethod<void>(0x737940, this, NewIndicatorType, InIndicatorVfxName);
}

void EARS::Modules::NPC::DisableHUDIndicator()
{
	MemUtils::CallClassMethod<void>(0x72D290, this);
}

EARS::Modules::NPCCrewComponent* EARS::Modules::NPC::GetCrewComponent() const
{
	hook::Type<uint32_t> CrewComponentIndex = hook::Type<uint32_t>(0x1131040);
	EARS::Framework::Component* FoundComp = MemUtils::CallClassMethod<EARS::Framework::Component*, const EARS::Modules::NPC*, uint32_t>(0x043B870, this, CrewComponentIndex);
	return (EARS::Modules::NPCCrewComponent*)FoundComp;
}

EARS::Modules::NPCUpgradeComponent* EARS::Modules::NPC::GetUpgradeComponent() const
{
	if (EARS::Modules::SimNPC* OwnSimNPC = m_OwnerSimNPC.GetPtr())
	{
		return OwnSimNPC->GetUpgradeComponent();
	}

	return nullptr;
}
