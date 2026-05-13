#include "CResourceHandler.h"

// C++
#include <assert.h>

uint8_t* RWS::CResourceHandler::CResourceLoadInfo::BindResourceData()
{
	assert(m_RawData);

	m_BoundData = m_RawData;

	return reinterpret_cast<uint8_t*>(m_BoundData);
}
