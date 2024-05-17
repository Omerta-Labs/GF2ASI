#pragma once

// CPP
#include <cstdint>

namespace PartedModel
{
	// Need to forward declare due to use in PartedModel::Preset
	struct Assembly;

	// need to forward declare due to use in PartedModel::Assembly
	struct AssemblyListHeader;

	struct Preset
	{
		const char* m_PresetName = nullptr;
		PartedModel::Assembly* m_Assembly = nullptr;
		uint32_t m_NumItemSubGroups = 0;
		void* m_ItemSubGroupsArr = nullptr;
		uint32_t m_NumPartSpecs = 0;
		void* m_PartSpecsArr = nullptr;
		uint32_t m_NumThemeRefs = 0;
		void* m_ThemeRefsArr = nullptr;
	};

	struct Assembly
	{
		uint32_t m_Counter = 0;
		uint32_t m_Flags = 0;
		const char* m_AssemblyName = nullptr;
		uint32_t m_NumParts = 0;
		void* m_PartsArr = nullptr;
		uint32_t m_NumItems = 0;
		void* m_ItemsArr = nullptr;
		uint32_t m_NumItemGroups = 0;
		void* m_ItemGroupsArr = nullptr;
		uint32_t m_NumThemes = 0;
		void* m_ThemesArr = nullptr;
		uint32_t m_NumBlenders = 0;
		void* m_BlendersArr = nullptr;
		uint32_t m_NumPresets = 0;
		PartedModel::Preset* m_PresetsArr = nullptr;
		uint32_t m_NumPresetGroups = 0;
		void* m_PresetGroupsArr = nullptr;
		PartedModel::AssemblyListHeader* m_AssemblyList = nullptr;
	};

	struct AssemblyListHeader
	{
		uint32_t m_Magic = 0;
		uint32_t m_Version = 0;
		uint32_t m_Flags = 0;
		uint32_t m_DataSize = 0;
		char m_Padding[64];
		uint32_t m_NumAssemblies = 0;
		PartedModel::Assembly* m_Assemblies = nullptr;
		char m_Padding2[4];
		PartedModel::AssemblyListHeader* m_NextHeader = nullptr;
	};
}

class PartedModelMgr
{
public:

	static PartedModelMgr* GetInstance();

	void* VTable_0 = nullptr;
	void* VTable_1 = nullptr;
	void* VTable_2 = nullptr;
	const char* m_MultiplayerPaletteName = nullptr;
	PartedModel::AssemblyListHeader* m_AssembliesList = nullptr;
};

