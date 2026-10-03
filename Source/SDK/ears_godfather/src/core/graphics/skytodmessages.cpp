#include "skytodmessages.h"

// C++
#include <math.h>

float EARS::Godfather::SetSkyMessage::GetSkyParamF(const SkyFloatParam InParam) const
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	return Index < NUM_SKY_FLOAT_PARAMS ? m_FloatParams[Index] : 0.0f;
}

void EARS::Godfather::SetSkyMessage::SetSkyParamF(const SkyFloatParam InParam, const float InValue)
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	if (Index < NUM_SKY_FLOAT_PARAMS)
	{
		m_FloatParams[Index] = InValue;
	}
}

const float* EARS::Godfather::SetSkyMessage::GetSkyParamC(const SkyColorParam InParam) const
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	return m_ColorParams[Index < NUM_SKY_COLOR_PARAMS ? Index : 0];
}

void EARS::Godfather::SetSkyMessage::SetSkyParamC(const SkyColorParam InParam, const float InColor[4])
{
	const uint32_t Index = static_cast<uint32_t>(InParam);
	if (Index >= NUM_SKY_COLOR_PARAMS)
	{
		return;
	}

	m_ColorParams[Index][0] = InColor[0];
	m_ColorParams[Index][1] = InColor[1];
	m_ColorParams[Index][2] = InColor[2];
	m_ColorParams[Index][3] = InColor[3];
}

void EARS::Godfather::SetSkyMessage::SetOrientation(const float InQuaternion[4])
{
	m_Orientation[0] = InQuaternion[0];
	m_Orientation[1] = InQuaternion[1];
	m_Orientation[2] = InQuaternion[2];
	m_Orientation[3] = InQuaternion[3];

	CalcSunDirection();
}

void EARS::Godfather::SetSkyMessage::CalcSunDirection()
{
	// The engine only renormalises when the quaternion has actually drifted
	const float Norm = (m_Orientation[0] * m_Orientation[0])
		+ (m_Orientation[1] * m_Orientation[1])
		+ (m_Orientation[2] * m_Orientation[2])
		+ (m_Orientation[3] * m_Orientation[3]);

	if (fabsf(Norm - 1.0f) > 0.0001f && Norm > 0.0f)
	{
		const float Scale = 1.0f / sqrtf(Norm);

		m_Orientation[0] *= Scale;
		m_Orientation[1] *= Scale;
		m_Orientation[2] *= Scale;
		m_Orientation[3] *= Scale;
	}

	// Rotate (0, 0, 1) by the quaternion - this is the third column of the rotation matrix
	const float X = m_Orientation[0];
	const float Y = m_Orientation[1];
	const float Z = m_Orientation[2];
	const float W = m_Orientation[3];

	float SunX = 2.0f * ((W * Y) + (Z * X));
	float SunY = 2.0f * ((Z * Y) - (W * X));
	float SunZ = 1.0f - (2.0f * ((X * X) + (Y * Y)));

	const float Length = sqrtf((SunX * SunX) + (SunY * SunY) + (SunZ * SunZ));
	if (Length > 0.0f)
	{
		const float Scale = 1.0f / Length;

		SunX *= Scale;
		SunY *= Scale;
		SunZ *= Scale;
	}

	SetSkyParamF(SkyFloatParam::SUN_DIR_X, SunX);
	SetSkyParamF(SkyFloatParam::SUN_DIR_Y, SunY);
	SetSkyParamF(SkyFloatParam::SUN_DIR_Z, SunZ);
}

void EARS::Godfather::SetSkyMessage::SetDefaultSkyParams()
{
	SetSkyParamF(SkyFloatParam::SUN_POWER, 0.41f);
	SetSkyParamF(SkyFloatParam::SUN_DIR_X, -0.684f);
	SetSkyParamF(SkyFloatParam::SUN_DIR_Y, -0.25f);
	SetSkyParamF(SkyFloatParam::SUN_DIR_Z, -0.684f);

	SetSkyParamF(SkyFloatParam::DIFFUSION_PEAK, 0.05f);
	SetSkyParamF(SkyFloatParam::DIFFUSION_EXPONENT, 350.0f);
	SetSkyParamF(SkyFloatParam::DIFFUSION_RAMP_UP, 11.5f);
	SetSkyParamF(SkyFloatParam::DIFFUSION_RAMP_DOWN, 12.5f);

	SetSkyParamF(SkyFloatParam::CLOUD_OPAQUE_DENSITY, 1.0f);
	SetSkyParamF(SkyFloatParam::SKY_DIFFUSION_EXP, 100.0f);
	SetSkyParamF(SkyFloatParam::SKY_DIFFUSION_EXP_LOW, 10.0f);
	SetSkyParamF(SkyFloatParam::HAZE_BAND_ANGLE, 0.2f);

	SetSkyParamF(SkyFloatParam::SKY_START_DIST, 0.0f);
	SetSkyParamF(SkyFloatParam::SKY_END_DIST, 1.0f);
	SetSkyParamF(SkyFloatParam::CLOUD_START_DIST, 0.0f);
	SetSkyParamF(SkyFloatParam::CLOUD_END_DIST, 1.0f);

	SetSkyParamF(SkyFloatParam::SKY_FOG_START_DIST, 0.0f);
	SetSkyParamF(SkyFloatParam::SKY_FOG_END_DIST, 0.0f);
	SetSkyParamF(SkyFloatParam::CLOUD_FOG_START_DIST, 0.0f);
	SetSkyParamF(SkyFloatParam::CLOUD_FOG_END_DIST, 0.0f);

	SetSkyParamF(SkyFloatParam::UNKNOWN_14, 500.0f);

	const float DiffusionColor[4] = { 0.3f, 0.2f, 0.2f, 1.0f };
	const float DiffusionColorLow[4] = { 0.5f, 0.3f, 0.2f, 1.0f };
	const float SkyColor[4] = { 0.5f, 0.5f, 1.0f, 1.0f };
	const float SkyColorLow[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

	SetSkyParamC(SkyColorParam::DIFFUSION_COLOR, DiffusionColor);
	SetSkyParamC(SkyColorParam::DIFFUSION_COLOR_LOW, DiffusionColorLow);
	SetSkyParamC(SkyColorParam::SKY_COLOR, SkyColor);
	SetSkyParamC(SkyColorParam::SKY_COLOR_LOW, SkyColorLow);

	// Identity transform and identity orientation
	for (uint32_t Row = 0; Row < 4; ++Row)
	{
		for (uint32_t Column = 0; Column < 4; ++Column)
		{
			m_Transform[Row][Column] = (Row == Column) ? 1.0f : 0.0f;
		}
	}

	m_Orientation[0] = 0.0f;
	m_Orientation[1] = 0.0f;
	m_Orientation[2] = 0.0f;
	m_Orientation[3] = 1.0f;
}
