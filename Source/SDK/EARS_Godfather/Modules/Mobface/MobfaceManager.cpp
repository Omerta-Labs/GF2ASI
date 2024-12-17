#include "MobfaceManager.h"

// addons
#include "Addons/Hook.h"

void EARS::Modules::MobfaceManager::BuildModelFromSavedData(const PlayerFamilyTree::FamilyTreeSlot SlotIndex, EARS::Modules::PartedAnimated* TargetModel)
{
	using namespace EARS::Modules;

	MemUtils::CallClassMethod<void, MobfaceManager*, PlayerFamilyTree::FamilyTreeSlot, PartedAnimated*>(0x9BF2F0, this, SlotIndex, TargetModel);
}

EARS::Modules::MobfaceManager* EARS::Modules::MobfaceManager::GetInstance()
{
	return *(EARS::Modules::MobfaceManager**)0x11298A0;
}
