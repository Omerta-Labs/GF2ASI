#include "Guid.h"

uint32_t EARS::Common::guid128_t::GetGuid32() const
{
	return d + 33 * (c + 33 * (b + 33 * a));
}
