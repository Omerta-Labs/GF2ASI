#include "lightingtodmessages.h"

float EARS::Godfather::LightingTODMessage::GetParam(const LightingTODParams InParam) const
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	return Index < NUM_LIGHTING_TOD_PARAMS ? m_Params[Index] : 0.0f;
}

void EARS::Godfather::LightingTODMessage::SetParam(const LightingTODParams InParam, const float InValue)
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	if (Index < NUM_LIGHTING_TOD_PARAMS)
	{
		m_Params[Index] = InValue;
	}
}

void EARS::Godfather::LightingTODMessage::SetDefaultParams()
{
	SetParam(LightingTODParams::MAIN_ATTEN, 0.8f);
	SetParam(LightingTODParams::ALL_ATTEN, 0.21f);
	SetParam(LightingTODParams::RIGID, 0.05f);
	SetParam(LightingTODParams::SKIN, 0.05f);
}
