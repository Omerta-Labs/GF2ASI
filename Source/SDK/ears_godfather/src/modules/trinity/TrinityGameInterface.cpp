#include "TrinityGameInterface.h"

namespace EARS::Modules
{
	TrinityGameInterface* TrinityGameInterface::GetInstance()
	{
		return *(EARS::Modules::TrinityGameInterface**)0x1129948;
	}
}
