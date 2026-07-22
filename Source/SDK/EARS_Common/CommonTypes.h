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

		template<class TValue, typename TKey>
		struct GetKeyFunc
		{
		public:

			static TKey* GetKey(const TValue& Value) = 0;
		};

		uint32_t HashMem_SDBM(const void* pVoidData, uint32_t dataLen);

		// Case-insensitive SDBM hash of a null-terminated string (engine addr 0x4DAFD0).
		// This is how RWS event ids are derived: id == HashString_SDBM(eventName).
		uint32_t HashString_SDBM(const char* pString);

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