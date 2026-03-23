#pragma once

// SDK
#include "SDK/EARS_Common/IAllocator.h"

namespace EARS::Common
{
	struct TVPContainer
	{
	public:

	private:
	
		EA::TagValuePair* m_TVP = nullptr;
		EA::Allocator::IAllocator* m_Allocator = nullptr;
	};
}