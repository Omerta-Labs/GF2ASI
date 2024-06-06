#include "CityManager.h"

EARS::Modules::CityManager* EARS::Modules::CityManager::GetInstance()
{
	return *(CityManager**)0x112A66C;
}
