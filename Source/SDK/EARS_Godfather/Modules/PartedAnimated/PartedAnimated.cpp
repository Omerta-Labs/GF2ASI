#include "PartedAnimated.h"

// Addons
#include "Addons/Hook.h"

// CPP
#include <string>

void EARS::Modules::PartedAnimated::ForceModelRebuild(EARS::Modules::PresetInstance* NewInstance, const char* NewPresetName)
{
	MemUtils::CallClassMethod<void, EARS::Modules::PartedAnimated*, void*, const char*>(0x09C58C0, this, NewInstance, NewPresetName);
}

void EARS::Modules::PartedAnimated::TrySwapModel(const char* AssemblyName, const char* PresetName)
{
	m_PartsAssemblyName.assign(AssemblyName);
	m_PartsPresetName.assign(PresetName);

	ForceModelRebuild(nullptr, PresetName);

	// we will revert back to original.
	// this ensures that when the game refreshes the model, it will revert back to appropriate model
	// TODO: This is very Player specific, and will probably break for other parted models.
	m_PartsPresetName.assign("mobface");
	m_PartsAssemblyName.assign("player");
}
