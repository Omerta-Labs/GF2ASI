#pragma once

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Common
	{
		struct guid128_t
		{
			uint32_t a, b, c, d = 0;
		};
	} // Common
} // EARS
