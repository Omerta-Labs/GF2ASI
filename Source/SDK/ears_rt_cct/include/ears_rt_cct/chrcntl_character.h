#pragma once

// SDK
#include "ears_common/hashtable.h"
#include "ears_rt_cct/cctnamenode.h"

// CPP
#include <stdint.h>

typedef EARS::Common::HashTableByValue<uint32_t, EA::CCT::CCTNameNode, 32> ChrCntl_NameNodeHash;

/**
 * Defines an animated Character at runtime.
 * Stored in game files, loaded at runtime
 */
class ChrCntl_ChrInfo_s
{
public:

	uint8_t m_Padding1[0x30];
	ChrCntl_NameNodeHash* m_NameNodeHash = nullptr;
};

namespace EA
{
	namespace CCT
	{
		// Get the name of the current state from the Characters info structure
		const char* Character_GetStateName(const ChrCntl_ChrInfo_s& InCharacter, const uint32_t ID);

		// Get the node of the current state from the Characters info structure
		const ChrCntl_AnimNode_s* Character_GetStateNode(const ChrCntl_ChrInfo_s& InCharacter, const uint32_t ID);

		// Fetch number of state nodes within a Character
		uint32_t Character_CountStateNodes(const ChrCntl_ChrInfo_s& InCharacter);

		// Fetch number of state names within a Character
		uint32_t Character_CountStateNames(const ChrCntl_ChrInfo_s& InCharacter);

		typedef bool(__cdecl* TEnumerateNodesCB)(int, const ChrCntl_AnimNode_s&, void*);
		int32_t Character_EnumerateStateNodes(const ChrCntl_ChrInfo_s& InCharacter, TEnumerateNodesCB Callback, void* InOutData);

		typedef bool(__cdecl* TEnumerateNamesCB)(int, const char*, void*);
		int32_t Character_EnumerateStateNames(const ChrCntl_ChrInfo_s& InCharacter, TEnumerateNamesCB Callback, void* InOutData);
	} // CCT
} // EA
