#pragma once

// SDK
#include "SDK/EARS_Common/TVPContainer.h"

// C++
#include <assert.h>
#include <cstdint>

template<typename TClass>
struct ManagedArray
{
public:

	uint32_t Size() const { return m_Size; }

	const TClass* operator[](const uint32_t index) const
	{
		assert(m_Array || index > m_Size);
		return &m_Array[index];
	}

private:

	TClass* m_Array;
	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;
	EARS::Common::TVPContainer m_AllocatorParams;
};
