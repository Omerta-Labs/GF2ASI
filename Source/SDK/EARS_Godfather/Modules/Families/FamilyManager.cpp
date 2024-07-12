#include "FamilyManager.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Families/Family.h"

void EARS::Modules::FamilyManager::Test()
{
	for (const EARS::Modules::Family* CurrentFamily : m_Families)
	{
		int z = 0;
	}

	for (const EARS::Modules::Family* CurrentFamily : m_StrategyFamilies)
	{
		int z = 0;
	}
}

EARS::Modules::FamilyManager* EARS::Modules::FamilyManager::GetInstance()
{
	return *(EARS::Modules::FamilyManager**)0x1129904;
}
