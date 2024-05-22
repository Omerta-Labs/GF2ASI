#pragma once

// CPP
#include <cstdint>
#include <functional>

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

	/**
	 * Utility function to iterate through all loaded assembly headers
	 * All const, we do not expect to modify any of the assemblies during iteration.
	 * All headers pass-by-ref, so none of them should be nullptr.
	 * Function does not exist in GF2 exe, merely utility header for us to use.
	 */
	typedef std::function<void(const PartedModel::AssemblyListHeader&)> TVisitAssemblyHeaderFunctor;
	void ForEachAssemblyHeader(const TVisitAssemblyHeaderFunctor& InFunction) const;

	// Access to the Parted Model Manager singleton instance
	static PartedModelMgr* GetInstance();


private:

	void* VTable_0 = nullptr;
	void* VTable_1 = nullptr;
	void* VTable_2 = nullptr;

	// Unsure, MP related
	const char* m_MultiplayerPaletteName = nullptr;

	// A list of loaded assemblies
	PartedModel::AssemblyListHeader* m_AssembliesList = nullptr;
};

