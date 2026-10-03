#include "MobfaceManager.h"

// addons
#include "Addons/Hook.h"

bool EARS::Modules::MobfaceManager::HasSavedData(const EARS::Modules::PlayerFamilyTree::FamilyTreeSlot SlotIndex) const
{
	const int32_t SlotIndexAsInt = (int32_t)SlotIndex;
	const uint32_t BitIndex = (uint32_t)(1 << (SlotIndexAsInt + 1));
	return HasApparelSaveData(BitIndex) || HasMobFaceSaveData(BitIndex);
}

void EARS::Modules::MobfaceManager::ResetMobfaceForSlot(const PlayerFamilyTree::FamilyTreeSlot SlotIndex)
{
	const int32_t SlotIndexAsInt = (int32_t)SlotIndex;
	const uint32_t BitIndex = (uint32_t)(1 << (SlotIndexAsInt + 1));

	ClearMobFaceSaveData(BitIndex);
	ClearApparelSaveData(BitIndex);
}

bool EARS::Modules::MobfaceManager::HasApparelSaveData(const uint32_t BitIndex) const
{
	return (m_SaveHeader.m_EditedInApparel & BitIndex) == BitIndex;
}

bool EARS::Modules::MobfaceManager::HasMobFaceSaveData(const uint32_t BitIndex) const
{
	return (m_SaveHeader.m_EditedInMobFace & BitIndex) == BitIndex;
}

void EARS::Modules::MobfaceManager::ClearMobFaceSaveData(const uint32_t BitIndex)
{
	m_SaveHeader.m_EditedInMobFace &= ~BitIndex;
}

void EARS::Modules::MobfaceManager::ClearApparelSaveData(const uint32_t BitIndex)
{
	m_SaveHeader.m_EditedInApparel &= ~BitIndex;
}

void EARS::Modules::MobfaceManager::BuildModelFromSavedData(const PlayerFamilyTree::FamilyTreeSlot SlotIndex, EARS::Modules::PartedAnimated* TargetModel)
{
	using namespace EARS::Modules;

	MemUtils::CallClassMethod<void, MobfaceManager*, PlayerFamilyTree::FamilyTreeSlot, PartedAnimated*>(0x9BF2F0, this, SlotIndex, TargetModel);
}

EARS::Modules::MobfaceManager* EARS::Modules::MobfaceManager::GetInstance()
{
	return *(EARS::Modules::MobfaceManager**)0x11298A0;
}
