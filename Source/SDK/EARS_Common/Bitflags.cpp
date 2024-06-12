#include "Bitflags.h"

Flags16::Flags16()
	: m_Flags(0)
{

}

Flags16::Flags16(const uint16_t InitialiseValue)
	: m_Flags(InitialiseValue)
{

}

void Flags16::Clear(const uint16_t Flag)
{
	m_Flags &= ~Flag;
}

void Flags16::ClearAll()
{
	m_Flags = 0;
}

uint16_t Flags16::GetAllFlags() const
{
	return m_Flags;
}

void Flags16::Set(const uint16_t Flag)
{
	m_Flags | Flag;
}

void Flags16::Set(const uint16_t Flag, bool bNewValue)
{
	(bNewValue ? Set(Flag) : Clear(Flag));
}

void Flags16::SetAllFlags(const uint16_t NewFlags)
{
	m_Flags = NewFlags;
}

bool Flags16::Test(const uint16_t Flag) const
{
	return (m_Flags & Flag) == Flag;
}

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
	return (m_Flags & Flag) == Flag;
}
