#include "UIHud.h"

// Addons
#include "Addons/Hook.h"

void EARS::Apt::UIHUD::Suppress()
{
	MemUtils::CallClassMethod<void, EARS::Apt::UIHUD*>(0x093C210, this);
}

void EARS::Apt::UIHUD::Unsuppress()
{
	MemUtils::CallClassMethod<void, EARS::Apt::UIHUD*>(0x093A2F0, this);
}

void EARS::Apt::UIHUD::AddCrew(EARS::Modules::NPC* CrewMember) const
{
	MemUtils::CallClassMethod<void, const EARS::Apt::UIHUD*, EARS::Modules::NPC*>(0x093A370, this, CrewMember);
}

void EARS::Apt::UIHUD::RemoveCrew(EARS::Modules::NPC* CrewMember) const
{
	MemUtils::CallClassMethod<void, const EARS::Apt::UIHUD*, EARS::Modules::NPC*>(0x0939550, this, CrewMember);
}

void EARS::Apt::UIHUD::AddCrewSpecialty(EARS::Modules::NPC* CrewNPC, uint32_t NewSpecialty)
{
	MemUtils::CallClassMethod<void, EARS::Apt::UIHUD*, EARS::Modules::NPC*, uint32_t>(0x09398E0, this, CrewNPC, NewSpecialty);
}

EARS::Apt::UIHUD* EARS::Apt::UIHUD::GetInstance()
{
	return *(EARS::Apt::UIHUD**)0x1129C4C;
}
