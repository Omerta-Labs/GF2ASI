#include "PlayerFamilyTree.h"

// Hooks
#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Components/NPC/NPCCrewComponent.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"

// CPP
#include <bitset>

EARS::Modules::PlayerFamilyTree::FamilyTreeSlot EARS::Modules::PlayerFamilyTree::FindTreeSlotIndex(const EARS::Modules::SimNPC* InSimNPC) const
{
	// NB: This is completed implementation of the engine function

	for (uint32_t i = 0; i < 7; i++)
	{
		const EARS::Modules::PlayerFamilyMember& Member = m_FamilyMembers[i];
		if (Member.IsSlotFilled() && Member.GetSimNPC() == InSimNPC)
		{
			return (FamilyTreeSlot)i;
		}
	}

	return FamilyTreeSlot::FAMILYTREE_SLOT_INVALID;
}

void EARS::Modules::PlayerFamilyTree::ForEachMember(const TVisitFamilyMemberFunctor& InFunction)
{
	for (uint32_t i = 0; i < 7; i++)
	{
		InFunction(m_FamilyMembers[i]);
	}
}

EARS::Modules::PlayerFamilyTree* EARS::Modules::PlayerFamilyTree::GetInstance()
{
	return *(EARS::Modules::PlayerFamilyTree**)0x112989C;
}

bool EARS::Modules::PlayerFamilyMember::HasSpecialty(const EARS::Modules::Specialties NewSpeciality) const
{
	std::bitset<32> SpecialityBits = m_Specialties;
	return SpecialityBits.test((uint32_t)NewSpeciality);
}

void EARS::Modules::PlayerFamilyMember::AddSpecialty(const EARS::Modules::Specialties NewSpeciality)
{
	std::bitset<32> SpecialityBits = m_Specialties;
	SpecialityBits[(uint32_t)NewSpeciality] = 1;
	m_Specialties = (uint32_t)SpecialityBits.to_ulong();

	OnSpecialitiesUpdated();
}

void EARS::Modules::PlayerFamilyMember::ToggleSpecialty(const EARS::Modules::Specialties NewSpeciality)
{
	std::bitset<32> SpecialityBits = m_Specialties;
	SpecialityBits[(uint32_t)NewSpeciality].flip();
	m_Specialties = (uint32_t)SpecialityBits.to_ulong();

	OnSpecialitiesUpdated();
}

void EARS::Modules::PlayerFamilyMember::RemoveSpecialty(const EARS::Modules::Specialties Speciality)
{
	std::bitset<32> SpecialityBits = m_Specialties;
	SpecialityBits[(uint32_t)Speciality] = 0;
	m_Specialties = (uint32_t)SpecialityBits.to_ulong();

	OnSpecialitiesUpdated();
}

bool EARS::Modules::PlayerFamilyMember::IsSlotFilled() const
{
	return (TestPlayerFamilyMemberFlags((uint32_t)Flags::FLAG_FAMILYMEMBER_SLOT_FILLED) == true);
}

void EARS::Modules::PlayerFamilyMember::JoinCrew()
{
	MemUtils::CallClassMethod<void, EARS::Modules::PlayerFamilyMember*>(0x090A660, this);
}

void EARS::Modules::PlayerFamilyMember::LeaveCrew()
{
	MemUtils::CallClassMethod<void, EARS::Modules::PlayerFamilyMember*>(0x090A700, this);
}

void EARS::Modules::PlayerFamilyMember::OnSpecialitiesUpdated()
{
	// TODO: Access SimNPC
	// TODO: Get CrewComponent
	// TODO: CrewComponent - Update Settings
	// TODO: Update HUD
	// TODO: If Medic, Init medic data

	MemUtils::CallClassMethod<void, EARS::Modules::PlayerFamilyMember*, uint32_t>(0x090A610, this, m_Specialties);

	if(EARS::Modules::SimNPC* FamilySimNPC = GetSimNPC())
	{
		if (EARS::Modules::NPC* FamilyNPC = FamilySimNPC->GetNPC())
		{
			// TODO: Does not work
			EARS::Modules::NPCCrewComponent* CrewComponent = FamilyNPC->GetCrewComponent();
			//CrewComponent->AddNewCrewSpecialty(m_Specialties);
		}
	}
}

bool EARS::Modules::PlayerFamilyMember::TestPlayerFamilyMemberFlags(uint32_t Flag) const
{
	return m_Flags.Test(Flag);
}
