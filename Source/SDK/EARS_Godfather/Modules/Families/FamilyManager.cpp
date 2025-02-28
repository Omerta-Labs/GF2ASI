#include "FamilyManager.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Families/Family.h"

const EARS::Modules::Family* EARS::Modules::FamilyManager::GetFamily(const uint32_t FamilyID) const
{
	for (const EARS::Modules::Family* CurrentFamily : m_Families)
	{
		if (CurrentFamily->GetFamilyID() == FamilyID)
		{
			return CurrentFamily;
		}
	}

	return nullptr;
}

void EARS::Modules::FamilyManager::ForEachFamily(const TVisitFamilyFunctor& InFunction)
{
	for (EARS::Modules::Family* CurrentFamily : m_Families)
	{
		InFunction(*CurrentFamily);
	}
}

void EARS::Modules::FamilyManager::ForEachStrategyFamily(const TVisitFamilyFunctor& InFunction)
{
	for (EARS::Modules::Family* CurrentFamily : m_StrategyFamilies)
	{
		InFunction(*CurrentFamily);
	}
}

EARS::Modules::FamilyManager* EARS::Modules::FamilyManager::GetInstance()
{
	return *(EARS::Modules::FamilyManager**)0x1129904;
}
