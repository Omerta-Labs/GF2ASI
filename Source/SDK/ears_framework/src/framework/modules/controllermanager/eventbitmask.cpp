#include "eventbitmask.h"

#include "Platform/MemUtils.h"

namespace EARS
{
	namespace Modules
	{
		void Bitmask::ClearAll()
		{
			MemUtils::CallClassMethod<void, Bitmask*>(0x4CDF80, this);
		}
	} // Modules
} // EARS
