#pragma once

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Common
	{
		struct guid128_t
		{
		public:
		
			// Operator overloads for equality
			inline bool operator==(const guid128_t& rhs) const { return (this->a == rhs.a && this->b == rhs.b && this->c == rhs.c && this->d == rhs.d); }
			inline bool operator!=(const guid128_t& rhs) const { return !(*this == rhs); }

			// TODO: Private these!
			uint32_t a, b, c, d = 0;
		};
	} // Common
} // EARS
