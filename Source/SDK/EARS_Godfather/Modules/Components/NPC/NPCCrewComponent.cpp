#include "NPCCrewComponent.h"

// Addons
#include "Addons/Hook.h"

void EARS::Modules::NPCCrewComponent::AddNewCrewSpecialty(const uint32_t Specialty)
{
	MemUtils::CallClassMethod<void, EARS::Modules::NPCCrewComponent*, uint32_t>(0x9B1F60, this, Specialty);
}

void EARS::Modules::NPCCrewComponent::InitMedic()
{
	
}
