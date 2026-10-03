#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Memory/Allocator.h"

// C++
#include <stdint.h>

namespace EARS::Allocator
{
	class GlobalHeapAllocator : public Allocator
	{
	public:

		// NB: In game exe they override global operators, these copy the code executed
		// Once we have fully reverse-engineered this class, we can probably properly implement these
		static void* OperatorNewArray(const uint32_t Size);
		static void OperatorDeleteArray(void* Data);

		static GlobalHeapAllocator* GetSingletonPtr();

	private:
	};
}

extern EARS::Allocator::Allocator* get_thread_new_allocator();
