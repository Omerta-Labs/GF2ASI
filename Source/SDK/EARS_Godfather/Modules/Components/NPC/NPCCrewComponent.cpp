#include "NPCCrewComponent.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Families/Family.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"

// Addons
#include "Addons/Hook.h"

void EARS::Modules::NPCCrewComponent::AddNewCrewSpecialty(const uint32_t Specialty)
{
	MemUtils::CallClassMethod<void, EARS::Modules::NPCCrewComponent*, uint32_t>(0x9B1F60, this, Specialty);
}

void EARS::Modules::NPCCrewComponent::InitMedic()
{
	EARS::Modules::Family* CurrentFamily = m_NPC->GetFamily();
	if (CurrentFamily == nullptr)
	{
		// missing family
		return;
	}

	if (m_CrewFlags.Test(0x800))
	{
		// likely already a medic
		return;
	}

	// Add medic into current family
	CurrentFamily->AddMedic(m_NPC);
	m_CrewFlags.Set(0x800);
}
