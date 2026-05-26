#pragma once

// SDK
#include "SDK/EARS_Common/IAllocator.h"

namespace EARS::Allocator
{
	class GlobalHeapAllocator
	{
	public:

		// NB: In game exe they override global operators, these copy the code executed
		// Once we have fully reverse-engineered this class, we can probably properly implement these
		static void* OperatorNewArray(const uint32_t Size);
		static void OperatorDeleteArray(void* Data);

	private:
	};
}
