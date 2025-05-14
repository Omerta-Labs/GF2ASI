#pragma once

// SDK
#include "SDK/EARS_Common/HashTable.h"
#include "SDK/EARS_RT_CCT/include/CCTNameNode.h"

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
	} // CCT
} // EA