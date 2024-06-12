#include "SimNPC.h"

// Hook
#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Components/NPC/NPCCrewComponent.h"

EARS::Modules::NPCCrewComponent* EARS::Modules::NPC::GetCrewComponent() const
{
	hook::Type<uint32_t> CrewComponentIndex = hook::Type<uint32_t>(0x1131040);
	EARS::Framework::Component* FoundComp = MemUtils::CallClassMethod<EARS::Framework::Component*, const EARS::Modules::NPC*, uint32_t>(0x043B870, this, CrewComponentIndex);
	return (EARS::Modules::NPCCrewComponent*)FoundComp;
}

String* EARS::Modules::SimNPC::GetName()
{
	return MemUtils::CallClassMethod<String*, EARS::Modules::SimNPC*>(0x0790610, this);
}
