#include "Guid.h"

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
