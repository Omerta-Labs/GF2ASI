#include "Family.h"

// Addons
#include "Addons/Hook.h"

// C++
#include <assert.h>

void EARS::Modules::Family::AddMedic(EARS::Modules::Sentient* NewMedic)
{
	MemUtils::CallClassMethod<void, EARS::Modules::Family*, EARS::Modules::Sentient*>(0x08C3C80, this, NewMedic);
}

float EARS::Modules::Family::ModifyBalance(const float Modifier, const LedgerItemType LedgerType)
{
	return MemUtils::CallClassMethod<float, EARS::Modules::Family*, float, LedgerItemType>(0x08C09A0, this, Modifier, LedgerType);
}

uint32_t EARS::Modules::Family::GetAllyFamilyID(const uint32_t Index) const
{
	assert(m_NumAllyFamilies > Index && "Expected to be less than number of current allies!");

	return m_FamilyAllyID[Index];
}

const String* EARS::Modules::Family::GetInternalName() const
{
	if (m_InternalName)
	{
		return &m_InternalName;
	}

	return nullptr;
}

const String* EARS::Modules::Family::GetSingularName() const
{
	if (m_SingularDisplayName)
	{
		return &m_SingularDisplayName;
	}

	return nullptr;
}

const String* EARS::Modules::Family::GetPluralName() const
{
	if (m_PluralDisplayName)
	{
		return &m_PluralDisplayName;
	}

	return nullptr;
}
