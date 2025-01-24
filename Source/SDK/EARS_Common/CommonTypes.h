#pragma once

// C++
#include <stdint.h>

// TODO: Unsure whether these lived here, this is just an assumption
namespace EARS
{
	namespace Common
	{
		template<class TType>
		struct CompareFunc
		{
		public:

			static bool Equal(const TType& Left, const TType& Right) = 0;
		};

		template<class TType>
		struct HashFunc
		{
		public:

			static uint32_t Hash(const TType& Value) = 0;
		};

		template<class TType>
		struct HashNext
		{
		public:

			static TType* GetHashNext(const TType& Value) = 0;
			static void SetHashNext(TType& Value, TType* Next) = 0;
		};

		template<class TType>
		struct GetKeyFunc
		{
		public:

			static TType* GetKey(const TType& Value) = 0;
		};

		uint32_t HashMem_SDBM(const void* pVoidData, uint32_t dataLen);

		// TODO: Make inline
		template<>
		struct CompareFunc<uint32_t>
		{
			static bool Equal(const uint32_t& Left, const uint32_t& Right) { return Left == Right; }
		};

		template<>
		struct HashFunc<uint32_t>
		{
			static uint32_t Hash(const uint32_t& Value)
			{
				return Value;
			}
		};
	}
}