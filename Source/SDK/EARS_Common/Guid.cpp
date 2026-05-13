#include "Guid.h"

// C++
#include <assert.h>

EARS::Common::guid32_t::guid32_t()
	: m_Value(0)
{
}

EARS::Common::guid32_t::guid32_t(uint32_t Value)
	: m_Value(Value)
{
}

EARS::Common::guid32_t::guid32_t(const guid32_t& Guid)
	: m_Value(Guid.m_Value)
{
}

EARS::Common::guid32_t::guid32_t(const guid128_t& Guid)
{
}

void EARS::Common::guid32_t::Clear()
{
	m_Value = 0;
}

void EARS::Common::guid32_t::Invalidate()
{
	Set(0xFE16702F);
}

void EARS::Common::guid32_t::Set(const uint32_t Value)
{
	m_Value = Value;
}

void EARS::Common::guid32_t::Set(const guid128_t& Guid)
{
	m_Value = Guid[3] + 33 * (Guid[2] + 33 * (Guid[1] + 33 * Guid[0]));
}

uint32_t EARS::Common::guid128_t::GetGuid32() const
{
	return d + 33 * (c + 33 * (b + 33 * a));
}

void EARS::Common::guid128_t::Clear()
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
}

bool EARS::Common::guid128_t::operator<(const guid128_t& rhs) const
{
	if (a < rhs.a)
	{
		return true;
	}
	else if (a > rhs.a)
	{
		return false;
	}

	if (b < rhs.b)
	{
		return true;
	}
	else if (b > rhs.b)
	{
		return false;
	}

	if (c < rhs.c)
	{
		return true;
	}
	else if (c > rhs.c)
	{
		return false;
	}

	return d < rhs.d;
}

uint32_t EARS::Common::guid128_t::operator[](const uint32_t index) const
{
	switch (index)
	{
		case 0: return a;
		case 1: return b;
		case 2: return c;
		case 3: return d;
		default: assert(false);
	}
}
