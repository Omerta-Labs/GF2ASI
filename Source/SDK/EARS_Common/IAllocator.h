#pragma once

// C++
#include <stdint.h>

namespace EA
{
	struct TagValuePair
	{
	public:

		TagValuePair()
			: m_Tag(0)
			, m_Next(nullptr)
		{
			// just empty 
			m_Value.m_Integer = 0;
		}

		TagValuePair(uint32_t InTag, uint32_t InValue)
			: m_Tag(InTag)
			, m_Next(nullptr)
		{
			m_Value.m_Size = InValue;
		}

		TagValuePair(uint32_t InTag, int32_t InValue)
			: m_Tag(InTag)
			, m_Next(nullptr)
		{
			m_Value.m_Integer = InValue;
		}

		TagValuePair(uint32_t InTag, const void* InValue)
			: m_Tag(InTag)
			, m_Next(nullptr)
		{
			m_Value.m_Pointer = InValue;
		}

	private:

		union InternalValue
		{
			int32_t m_Integer;
			uint32_t m_Size;
			float m_Float;
			const void* m_Pointer;
		};

		uint32_t m_Tag = 0;
		InternalValue m_Value;
		EA::TagValuePair* m_Next = nullptr;
	};

	namespace Allocator
	{
		class IAllocator
		{
		public:

			virtual void* Alloc(uint32_t Size, const EA::TagValuePair& InPair) = 0;
			virtual void Free(void* Mem, uint32_t Size) = 0;
			virtual int AddRef() = 0;
			virtual int Release() = 0;

			// destructor is at the end of the vtable
			virtual ~IAllocator() = 0;

		private:
		};
	}
}