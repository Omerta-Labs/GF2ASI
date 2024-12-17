#pragma once

// C++
#include <stdint.h>

struct String
{
public:

	String(const char* InSrc);
	~String();

	const char* c_str() const { return m_pCStr; }

	void assign(const char* InSrc);

private:

	typedef void (*StringAllocator_Free)(String* pThis);

	char* m_pCStr = nullptr;
	uint32_t m_Length = 0;
	uint32_t m_AllocatedLength = 0;
	uint32_t m_StringAllocateFreeFunc = 0;
};
