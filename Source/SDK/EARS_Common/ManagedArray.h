#pragma once

// C++
#include <stdint.h>

template<typename TClass>
struct ManagedArray
{
public:

private:

	TClass* m_Array;
	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;

};