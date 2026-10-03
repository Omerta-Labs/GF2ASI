#include "EARSLightTODMessages.h"

float EARS::Modules::TOD::EARSLightMessage::GetEARSLightParam(const EARSLightParam InParam) const
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	return Index < NUM_EARS_LIGHT_PARAMS ? m_Params[Index] : 0.0f;
}

void EARS::Modules::TOD::EARSLightMessage::SetEARSLightParam(const EARSLightParam InParam, const float InValue)
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	if (Index < NUM_EARS_LIGHT_PARAMS)
	{
		m_Params[Index] = InValue;
	}
}

void EARS::Modules::TOD::EARSLightMessage::SetColor(const float InColor[4])
{
	m_Color[0] = InColor[0];
	m_Color[1] = InColor[1];
	m_Color[2] = InColor[2];
	m_Color[3] = InColor[3];
}

void EARS::Modules::TOD::EARSLightMessage::SetShadowColor(const float InColor[4])
{
	m_ShadowColor[0] = InColor[0];
	m_ShadowColor[1] = InColor[1];
	m_ShadowColor[2] = InColor[2];
	m_ShadowColor[3] = InColor[3];
}

void EARS::Modules::TOD::EARSLightMessage::SetDefaultParams()
{
	SetEARSLightParam(EARSLightParam::CONE_ANGLE, 45.0f);
	SetEARSLightParam(EARSLightParam::RADIUS, 10.0f);
	SetEARSLightParam(EARSLightParam::INTENSITY, 1.0f);
	SetEARSLightParam(EARSLightParam::FULLBRIGHT_ANGLE, 0.0f);
	SetEARSLightParam(EARSLightParam::FULLBRIGHT_RADIUS, 0.0f);
	SetEARSLightParam(EARSLightParam::DIFFUSE_AMBIENT, 0.0f);
	SetEARSLightParam(EARSLightParam::SPEC_SCALE, 1.0f);
	SetEARSLightParam(EARSLightParam::UNKNOWN_7, -0.0042f);
	SetEARSLightParam(EARSLightParam::SHADOW_DENSITY, 0.7f);
	SetEARSLightParam(EARSLightParam::UNKNOWN_9, 0.000005f);

	const float Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const float ShadowColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	SetColor(Color);
	SetShadowColor(ShadowColor);

	for (uint32_t Row = 0; Row < 4; ++Row)
	{
		for (uint32_t Column = 0; Column < 4; ++Column)
		{
			m_Transform[Row][Column] = (Row == Column) ? 1.0f : 0.0f;
		}
	}

	m_bUseAuthoredTransform = true;
}
