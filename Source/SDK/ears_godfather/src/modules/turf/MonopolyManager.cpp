#include "MonopolyManager.h"

namespace EARS
{
	namespace Modules
	{
		/* static */
		MonopolyManager* MonopolyManager::GetInstance()
		{
			return *(MonopolyManager**)0x112A590;
		}
	}
}