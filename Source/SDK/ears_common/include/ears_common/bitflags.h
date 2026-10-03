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

// Definitions were in a separate Bitflags.cpp here; the original is header-only
// (the manifest has bitflags.h and no .cpp), and the flag accessors show up in a
// dozen unrelated objects, so they were inline in this header.
inline Flags16::Flags16()
	: m_Flags(0)
{

}

inline Flags16::Flags16(const uint16_t InitialiseValue)
	: m_Flags(InitialiseValue)
{

}

inline void Flags16::Clear(const uint16_t Flag)
{
	m_Flags &= ~Flag;
}

inline void Flags16::ClearAll()
{
	m_Flags = 0;
}

inline uint16_t Flags16::GetAllFlags() const
{
	return m_Flags;
}

inline void Flags16::Set(const uint16_t Flag)
{
	m_Flags |= Flag;
}

inline void Flags16::Set(const uint16_t Flag, bool bNewValue)
{
	(bNewValue ? Set(Flag) : Clear(Flag));
}

inline void Flags16::SetAllFlags(const uint16_t NewFlags)
{
	m_Flags = NewFlags;
}

inline bool Flags16::Test(const uint16_t Flag) const
{
	return (m_Flags & Flag) == Flag;
}

inline Flags32::Flags32()
	: m_Flags(0)
{

}

inline Flags32::Flags32(const uint32_t InitialiseValue)
	: m_Flags(InitialiseValue)
{

}

inline void Flags32::Clear(const uint32_t Flag)
{
	m_Flags &= ~Flag;
}

inline void Flags32::ClearAll()
{
	m_Flags = 0;
}

inline uint32_t Flags32::GetAllFlags() const
{
	return m_Flags;
}

inline void Flags32::Set(const uint32_t Flag)
{
	m_Flags |= Flag;
}

inline void Flags32::Set(const uint32_t Flag, bool bNewValue)
{
	(bNewValue ? Set(Flag) : Clear(Flag));
}

inline void Flags32::SetAllFlags(const uint32_t NewFlags)
{
	m_Flags = NewFlags;
}

inline bool Flags32::Test(const uint32_t Flag) const
{
	return (m_Flags & Flag) == Flag;
}
