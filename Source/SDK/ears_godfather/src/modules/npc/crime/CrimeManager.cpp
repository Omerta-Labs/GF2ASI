#include "CrimeManager.h"

// Hooks
#include "Addons/Hook.h"

void EARS::Modules::CrimeManager::CalmPoliceTowardsCorleones()
{
	MemUtils::CallClassMethod<void, EARS::Modules::CrimeManager*>(0x07872C0, this);
}

EARS::Modules::CrimeManager* EARS::Modules::CrimeManager::GetInstance()
{
	return *(EARS::Modules::CrimeManager**)0x112AA1C;
}
