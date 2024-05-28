#include "Bitflags.h"

Flags32::Flags32()
	: m_Flags(0)
{

}

Flags32::Flags32(const uint32_t InitialiseValue)
	: m_Flags(InitialiseValue)
{

}

void Flags32::Clear(const uint32_t Flag)
{
	m_Flags &= ~Flag;
}

void Flags32::ClearAll()
{
	m_Flags = 0;
}

uint32_t Flags32::GetAllFlags() const
{
	return m_Flags;
}

void Flags32::Set(const uint32_t Flag)
{
	m_Flags |= Flag;
}

void Flags32::Set(const uint32_t Flag, bool bNewValue)
{
	(bNewValue ? Set(Flag) : Clear(Flag));
}

void Flags32::SetAllFlags(const uint32_t NewFlags)
{
	m_Flags = NewFlags;
}

bool Flags32::Test(const uint32_t Flag) const
{
	return (m_Flags & Flag) == 0;
}