#include "GlobalHeapAllocator.h"

#include "addons/hook.h"

namespace EARS::Allocator
{
	void* GlobalHeapAllocator::OperatorNewArray(const uint32_t Size)
	{
		return MemUtils::CallCdeclMethod<void*, uint32_t>(0x9C8E80, Size);
	}

	void GlobalHeapAllocator::OperatorDeleteArray(void* Data)
	{
		MemUtils::CallCdeclMethod<void*>(0x9C8F10, Data);
	}
}
