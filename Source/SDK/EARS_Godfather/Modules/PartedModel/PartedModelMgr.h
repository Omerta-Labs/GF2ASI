#pragma once

// CPP
#include <cstdint>

namespace PartedModel
{
	struct Assembly
	{
		uint32_t m_Counter = 0;
		uint32_t m_Flags = 0;
		const char* m_AssemblyName = nullptr;

		// TODO:
		/*unsigned int num_parts;
		$AB8B766467CFB55D6D754098E3F4D038 ___u4;
		unsigned int num_items;
		$E12E6A5651A2378198577F9527425D04 ___u6;
		unsigned int num_item_groups;
		$D0279ACA3679250E8E04EE865C572341 ___u8;
		unsigned int num_themes;
		$C85F4EC0C9DE493C51A059482EEC08ED ___u10;
		unsigned int num_blenders;
		$89277CC7C3D268A5C809531B0C5741C1 ___u12;
		unsigned int num_presets;
		$8AA54B8F409B67B3FA5A64FE2E64CBD1 ___u14;
		unsigned int num_preset_groups;
		$79E3D0357ED77B57F9B3F3C9B199121D ___u16;
		PartedModel::AssemblyListHeader* pAssemblyList;*/
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

