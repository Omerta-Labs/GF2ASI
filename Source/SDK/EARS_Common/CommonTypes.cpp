#include "SDK/EARS_Common/CommonTypes.h"

uint32_t EARS::Common::HashMem_SDBM(const void* pVoidData, uint32_t dataLen)
{
	const uint8_t* AsBytes = (uint8_t*)pVoidData;
	uint32_t Hash = 0;
	if (pVoidData)
	{
		for (uint32_t i = 0; i < dataLen; i++)
		{
			Hash = 0x1003F * Hash + AsBytes[i];
		}
	}
	return Hash;
}

uint32_t EARS::Common::HashString_SDBM(const char* pString)
{
	uint32_t Hash = 0;
	if (pString)
	{
		for (const char* Cursor = pString; *Cursor; ++Cursor)
		{
			uint32_t Char = static_cast<uint8_t>(*Cursor);

			// Fold uppercase A-Z to lowercase so the hash is case-insensitive.
			if (Char - 'A' <= 'Z' - 'A')
			{
				Char += 0x20;
			}

			Hash = 0x1003F * Hash + Char;
		}
	}
	return Hash;
}