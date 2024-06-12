#pragma once

// CPP
#include <stdint.h>

struct Flags16
{
public:

	Flags16();
	Flags16(const uint16_t InitialiseValue);

	void Clear(const uint16_t Flag);

	void ClearAll();

	uint16_t GetAllFlags() const;

	void Set(const uint16_t Flag);

	void Set(const uint16_t Flag, bool bNewValue);

	void SetAllFlags(const uint16_t NewFlags);

	bool Test(const uint16_t Flag) const;

private:

	// Underlying integer to store the flags
	uint16_t m_Flags = 0;
};

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

	// Underlying integer to store the flags
	uint32_t m_Flags = 0;
};
