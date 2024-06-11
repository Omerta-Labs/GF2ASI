#include "PlayerFamilyTree.h"

// CPP
#include <bitset>

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

void EARS::Modules::PlayerFamilyMember::OnSpecialitiesUpdated()
{
	// TODO: Access SimNPC
	// TODO: Get CrewComponent
	// TODO: CrewComponent - Update Settings
	// TODO: Update HUD
	// TODO: If Medic, Init medic data
}
