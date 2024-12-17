#include "String.h"

// SH
#include "addons/Hook.h"

// CPP
#include <cstring>

String::String(const char* InSrc)
{
	if (InSrc && *InSrc)
	{
		// figure out length of string
		const int SrcLength = strlen(InSrc);
		m_Length = SrcLength;
		m_AllocatedLength = SrcLength;

		// allocate string and copy source
		m_pCStr = MemUtils::CallCdeclMethod<char*, uint16_t>(0x9C8E50, m_Length + 1);
		memcpy_s(m_pCStr, m_Length, InSrc, m_Length);

		// snip end of string
		m_pCStr[m_Length] = 0;
		m_StringAllocateFreeFunc = 0x4D38D0;
	}
	else
	{
		m_pCStr = nullptr;
		m_AllocatedLength = 0;
		m_Length = 0;
	}
}

String::~String()
{
	if (m_pCStr)
	{
		// TODO: This is actually called using m_Unk0, as its a function pointer
		// For parity with GF2 exe, its probably best to follow suite
		MemUtils::CallCdeclMethod<int, const char*>(0x4D38D0, m_pCStr);
	}
}

void String::assign(const char* InSrc)
{
	// TODO: Move code from gf2 into asi
	MemUtils::CallClassMethod<void, String*, const char*>(0x4D3D90, this, InSrc);
}
