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