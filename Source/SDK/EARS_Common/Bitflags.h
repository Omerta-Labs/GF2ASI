#pragma once

// CPP
#include <stdint.h>

struct Flags32
{
public:

	Flags32();
	Flags32(const uint32_t InitialiseValue);

	void Clear(const uint32_t Flag);

	void ClearAll();

	uint32_t GetAllFlags() const;

	void Set(const uint32_t Flag);

	void Set(const uint32_t Flag, bool bNewValue);

	void SetAllFlags(const uint32_t NewFlags);

	bool Test(const uint32_t Flag) const;

private:
	uint32_t m_Flags = 0;
};
