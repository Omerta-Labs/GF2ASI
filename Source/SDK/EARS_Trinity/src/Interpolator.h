#pragma once

// C++
#include <stdint.h>

namespace EA::Trinity
{
	struct Interpolator
	{
	public:

		float m_CurrentTime = 0.0f;
		int16_t m_CurrentKey = 0;
		int16_t m_NumKeys = 0;
		int16_t m_KeyType = 0;
		int16_t m_Padding = 0;
		char m_Padding_0[0x4];	// TODO: This is a union
	};

	static_assert(sizeof(EA::Trinity::Interpolator) == 0x10);
}