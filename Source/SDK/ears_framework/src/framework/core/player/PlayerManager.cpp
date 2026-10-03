#include "PlayerManager.h"

// CPP
#include <cstdint>

namespace EARS
{
	namespace Framework
	{
		BasePlayer* PlayerManager::GetPlayer(const uint32_t PlayerID) const
		{
			// TODO: Could probably validate this by checking whether we are within range?
			return m_PlayerArray[PlayerID];
		}

		PlayerManager* PlayerManager::GetInstance()
		{
			return *(PlayerManager**)0x12233A0;
		}
	} // Framework
} // EARS
