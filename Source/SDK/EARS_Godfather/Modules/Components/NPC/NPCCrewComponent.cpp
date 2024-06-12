#include "NPCCrewComponent.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Families/Family.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/UI/UIHud.h"

// Addons
#include "Addons/Hook.h"

void EARS::Modules::NPCCrewComponent::AddNewCrewSpecialty(const uint32_t Specialty)
{
	m_CrewSettings.Set((uint16_t)Specialty);

	if (IsHiredCrew())
	{
		if (EARS::Apt::UIHUD* HUD = EARS::Apt::UIHUD::GetInstance())
		{
			HUD->AddCrewSpecialty(m_NPC, Specialty);
		}
	}

	if (m_CrewSettings.Test(0x40)) // query medic specialty
	{
		InitMedic();
	}
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

bool EARS::Modules::NPCCrewComponent::IsHiredCrew() const
{
	return m_CrewLeaderSentient.IsValid();
}
