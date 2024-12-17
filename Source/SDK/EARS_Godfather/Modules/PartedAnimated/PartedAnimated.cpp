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
}
