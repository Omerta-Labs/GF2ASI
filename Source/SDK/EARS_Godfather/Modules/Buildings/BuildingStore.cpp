#include "BuildingStore.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	void BuildingStore::ChangeOwnership(uint32_t NewFamilyID, const bool bShowFanfare, const void* Header, const bool bExtortion)
	{
		MemUtils::CallClassMethod<void, BuildingStore*, uint32_t, const bool, const void*, const bool>(0x06B68C0, this, NewFamilyID, bShowFanfare, Header, bExtortion);
	}
}
