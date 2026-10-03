#include "directionalshadowparams.h"

void EARS::DirectionalShadowParamData::Reset()
{
	m_Direction[0] = 0.0f;
	m_Direction[1] = 0.0f;
	m_Direction[2] = 0.0f;
	m_Direction[3] = 0.0f;

	m_Range = 7.0f;
	m_Height = 80.0f;
	m_Flags = static_cast<uint32_t>(DirectionalShadowFlags::DIRECTIONAL_SHADOW_NEVER_AUTHORED);
}

void EARS::DirectionalShadowParamData::Validate()
{
	m_Flags &= ~static_cast<uint32_t>(DirectionalShadowFlags::DIRECTIONAL_SHADOW_NEVER_AUTHORED);
}

bool EARS::DirectionalShadowParamData::IsInvalid() const
{
	return (m_Flags & static_cast<uint32_t>(DirectionalShadowFlags::DIRECTIONAL_SHADOW_NEVER_AUTHORED)) != 0;
}

bool EARS::DirectionalShadowParamData::UsesAuthoredDirection() const
{
	return (m_Flags & static_cast<uint32_t>(DirectionalShadowFlags::DIRECTIONAL_SHADOW_USE_AUTHORED_DIRECTION)) != 0;
}
