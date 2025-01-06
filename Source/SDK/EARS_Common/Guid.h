#pragma once

// SDK
#include "SDK/EARS_Common/CommonTypes.h"

// CPP
#include <stdlib.h>

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

		// TODO: Demote to inline header

		template<>
		struct HashFunc<guid128_t>
		{
			static uint32_t Hash(const guid128_t& Value)
			{
				// NB: PC version swaps from little endian to big endian.
				// No idea why, and no idea if its at this level or within HashMem_SDBM.

				guid128_t Copied = Value;
				Copied.a = _byteswap_ulong(Copied.a);
				Copied.b = _byteswap_ulong(Copied.b);
				Copied.c = _byteswap_ulong(Copied.c);
				Copied.d = _byteswap_ulong(Copied.d);
				return HashMem_SDBM(&Copied, sizeof(guid128_t));
			}
		};

		template<>
		struct CompareFunc<guid128_t>
		{
			static bool Equal(const guid128_t& Left, const guid128_t& Right) { return Left == Right; }
		};
	} // Common
} // EARS
