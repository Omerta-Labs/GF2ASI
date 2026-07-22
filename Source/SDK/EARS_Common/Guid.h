#pragma once

// SDK
#include "SDK/EARS_Common/CommonTypes.h"

// CPP
#include <stdlib.h>

namespace EARS
{
	namespace Common
	{
		// forward declares
		struct guid128_t;

		struct guid32_t
		{
		public:

			guid32_t();
			explicit guid32_t(uint32_t Value);
			guid32_t(const guid32_t& Guid);
			explicit guid32_t(const guid128_t& Guid);

			void Clear();

			void Invalidate();

			void Set(const uint32_t Value);
			void Set(const guid128_t& Guid);

			inline bool IsClear() const { return m_Value == 0; }
			inline bool IzZero() const { return m_Value == 0; }
			inline bool IsValid() const { return m_Value != 0xFE16702F; }
			inline uint32_t ToUint32() const { return m_Value; }

			// Operator overloads for equality
			inline bool operator==(const guid32_t& rhs) const { return (this->m_Value == rhs.m_Value); }
			inline bool operator!=(const guid32_t& rhs) const { return !(*this == rhs); }
			bool operator<(const guid32_t& rhs)  const { return (this->m_Value < rhs.m_Value); }

		private:

			uint32_t m_Value = 0;
		};

		struct guid128_t
		{
		public:
		
			/* used to pack the guid128_t into a guid32_t */
			uint32_t GetGuid32() const;

			/* reset the guid128_t */
			void Clear();
		
			// Operator overloads for equality
			inline bool operator==(const guid128_t& rhs) const { return (this->a == rhs.a && this->b == rhs.b && this->c == rhs.c && this->d == rhs.d); }
			inline bool operator!=(const guid128_t& rhs) const { return !(*this == rhs); }
			bool operator<(const guid128_t& rhs) const;

			// Operator overloads for accessing
			uint32_t operator[](const uint32_t index) const;

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
