#pragma once

// C++
#include <stdint.h>

struct String
{
public:

	String(const char* InSrc);
	~String();

	void assign(const char* InSrc);

	// inline
	const char* c_str() const { return m_pCStr; }
	bool IsValid() const { return m_Length != 0; }

	operator bool() const { return IsValid(); }

private:

	typedef void (*StringAllocator_Free)(String* pThis);

	char* m_pCStr = nullptr;
	uint32_t m_Length = 0;
	uint32_t m_AllocatedLength = 0;
	uint32_t m_StringAllocateFreeFunc = 0;
};
