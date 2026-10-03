#include "gfshaderdata.h"

namespace
{
	// Singleton<EARS::Godfather::GlobalCharLightingData> instance pointer
	constexpr uintptr_t ADDR_GLOBAL_CHAR_LIGHTING_DATA = 0x112DBA4;

	// Singleton<EARS::Godfather::GlobalSkyData> instance pointer
	constexpr uintptr_t ADDR_GLOBAL_SKY_DATA = 0x11318D4;

	inline void CopyVec4(float* OutDest, const float* InSrc)
	{
		OutDest[0] = InSrc[0];
		OutDest[1] = InSrc[1];
		OutDest[2] = InSrc[2];
		OutDest[3] = InSrc[3];
	}

	inline void SetVec4(float* OutDest, const float InX, const float InY, const float InZ, const float InW)
	{
		OutDest[0] = InX;
		OutDest[1] = InY;
		OutDest[2] = InZ;
		OutDest[3] = InW;
	}
}

//
// GlobalCharLightingData
//

EARS::Godfather::GlobalCharLightingData* EARS::Godfather::GlobalCharLightingData::GetInstance()
{
	return *reinterpret_cast<GlobalCharLightingData**>(ADDR_GLOBAL_CHAR_LIGHTING_DATA);
}

const float* EARS::Godfather::GlobalCharLightingData::GetSpecLightDirection(const uint32_t InIndex) const
{
	return m_SpecLightDirection[InIndex < NUM_SPEC_LIGHTS ? InIndex : 0];
}

const float* EARS::Godfather::GlobalCharLightingData::GetWorldSpaceSpecLightDirection(const uint32_t InIndex) const
{
	return m_WorldSpaceSpecLightDirection[InIndex < NUM_SPEC_LIGHTS ? InIndex : 0];
}

float EARS::Godfather::GlobalCharLightingData::GetSpecLightIntensity(const uint32_t InIndex) const
{
	return InIndex < NUM_SPEC_LIGHTS ? m_SpecLight[InIndex][0] : 0.0f;
}

float EARS::Godfather::GlobalCharLightingData::GetSpecLightExponent(const uint32_t InIndex) const
{
	return InIndex < NUM_SPEC_LIGHTS ? m_SpecLight[InIndex][1] : 0.0f;
}

void EARS::Godfather::GlobalCharLightingData::SetRimLight(const float InColorAndIntensity[4])
{
	CopyVec4(m_RimLight, InColorAndIntensity);
}

void EARS::Godfather::GlobalCharLightingData::SetMovieLight(const float InColorAndIntensity[4])
{
	CopyVec4(m_MovieLight, InColorAndIntensity);
}

void EARS::Godfather::GlobalCharLightingData::SetRimLightParams(const float InParams[4])
{
	CopyVec4(m_RimLightParams, InParams);
}

void EARS::Godfather::GlobalCharLightingData::SetRimLightDiffuseSpecParams(const float InParams[4])
{
	CopyVec4(m_RimLightDiffuseSpecParams, InParams);
}

void EARS::Godfather::GlobalCharLightingData::SetRimLightDirection(const float InDirection[4])
{
	CopyVec4(m_RimLightDirection, InDirection);
	m_bWorldSpaceLightDirectionsComputed = false;
}

void EARS::Godfather::GlobalCharLightingData::SetMovieLightDirection(const float InDirection[4])
{
	CopyVec4(m_MovieLightDirection, InDirection);
	m_bWorldSpaceLightDirectionsComputed = false;
}

void EARS::Godfather::GlobalCharLightingData::SetSpecLightDirection(const uint32_t InIndex, const float InDirection[4])
{
	if (InIndex >= NUM_SPEC_LIGHTS)
	{
		return;
	}

	CopyVec4(m_SpecLightDirection[InIndex], InDirection);
	m_bWorldSpaceLightDirectionsComputed = false;
}

void EARS::Godfather::GlobalCharLightingData::SetSpecLightIntensity(const uint32_t InIndex, const float InIntensity)
{
	if (InIndex < NUM_SPEC_LIGHTS)
	{
		m_SpecLight[InIndex][0] = InIntensity;
	}
}

void EARS::Godfather::GlobalCharLightingData::SetSpecLightExponent(const uint32_t InIndex, const float InExponent)
{
	if (InIndex < NUM_SPEC_LIGHTS)
	{
		m_SpecLight[InIndex][1] = InExponent;
	}
}

void EARS::Godfather::GlobalCharLightingData::SetWorldSpaceLightDirectionsComputed(const bool bInComputed)
{
	m_bWorldSpaceLightDirectionsComputed = bInComputed;
}

void EARS::Godfather::GlobalCharLightingData::RestoreDefaults()
{
	SetVec4(m_RimLight, 0.65f, 0.65f, 1.0f, 1.0f);
	SetVec4(m_MovieLight, 0.8f, 0.8f, 0.5f, 0.1f);
	SetVec4(m_RimLightParams, 10.0f, 15.0f, 0.3f, 0.08f);
	SetVec4(m_RimLightDiffuseSpecParams, 0.0f, 0.0f, 0.0f, 0.0f);

	SetVec4(m_RimLightDirection, 1.0f, 0.5f, 0.65f, 0.0f);
	SetVec4(m_MovieLightDirection, -0.03f, -0.01f, -0.05f, 0.0f);

	SetVec4(m_SpecLightDirection[0], 0.6f, -0.15f, -0.7f, 0.0f);
	SetVec4(m_SpecLightDirection[1], -0.3f, 0.02f, -0.76f, 0.0f);

	SetVec4(m_SpecLight[0], 0.2f, 0.25f, 0.0f, 0.0f);
	SetVec4(m_SpecLight[1], 2.15f, 2.15f, 0.0f, 0.0f);

	m_bWorldSpaceLightDirectionsComputed = false;
}

//
// GlobalSkyData
//

EARS::Godfather::GlobalSkyData* EARS::Godfather::GlobalSkyData::GetInstance()
{
	return *reinterpret_cast<GlobalSkyData**>(ADDR_GLOBAL_SKY_DATA);
}

void EARS::Godfather::GlobalSkyData::SetSunDirAndPower(const float InX, const float InY, const float InZ, const float InPower)
{
	SetVec4(m_SunDirAndPower, InX, InY, InZ, InPower);
}

void EARS::Godfather::GlobalSkyData::SetDiffusionControl(const float InPeakDensity, const float InExponent, const float InRampUpRate, const float InRampDownRate)
{
	SetVec4(m_DiffusionControl, InPeakDensity, InExponent, InRampUpRate, InRampDownRate);
}

void EARS::Godfather::GlobalSkyData::SetDiffusionColor(const float InColor[4])
{
	CopyVec4(m_DiffusionColor, InColor);
}

void EARS::Godfather::GlobalSkyData::SetDiffusionColorLow(const float InColor[4])
{
	CopyVec4(m_DiffusionColorLow, InColor);
}

void EARS::Godfather::GlobalSkyData::SetSkyColor(const float InColor[4])
{
	CopyVec4(m_SkyColor, InColor);
}

void EARS::Godfather::GlobalSkyData::SetSkyColorLow(const float InColor[4])
{
	CopyVec4(m_SkyColorLow, InColor);
}

void EARS::Godfather::GlobalSkyData::SetSkyDomeFogParams(const float InDomeStart, const float InDomeEnd, const float InFogStart, const float InFogEnd)
{
	m_SkyDomeFogParams[0] = InDomeStart;
	m_SkyDomeFogParams[1] = InFogStart;
	m_SkyDomeFogParams[2] = (InDomeEnd == InDomeStart) ? 1.0f : ((InFogEnd - InFogStart) / (InDomeEnd - InDomeStart));
	m_SkyDomeFogParams[3] = 0.0f;
}

void EARS::Godfather::GlobalSkyData::SetSkyCloudsFogParams(const float InCloudStart, const float InCloudEnd, const float InFogStart, const float InFogEnd)
{
	m_SkyCloudsFogParams[0] = InCloudStart;
	m_SkyCloudsFogParams[1] = InFogStart;
	m_SkyCloudsFogParams[2] = (InCloudEnd == InCloudStart) ? 1.0f : ((InFogEnd - InFogStart) / (InCloudEnd - InCloudStart));
	m_SkyCloudsFogParams[3] = 0.0f;
}

const float* EARS::Godfather::GlobalSkyData::GetCloudScrollParams(const uint32_t InLayer) const
{
	return m_CloudScrollParams[InLayer < NUM_SKY_SHADER_LAYERS ? InLayer : 0];
}

const float* EARS::Godfather::GlobalSkyData::GetAlphaScrollParams(const uint32_t InLayer) const
{
	return m_AlphaScrollParams[InLayer < NUM_SKY_SHADER_LAYERS ? InLayer : 0];
}

const float* EARS::Godfather::GlobalSkyData::GetTintColor(const uint32_t InLayer) const
{
	return m_TintColor[InLayer < NUM_SKY_SHADER_LAYERS ? InLayer : 0];
}

void EARS::Godfather::GlobalSkyData::SetCloudScrollParams(const uint32_t InLayer, const float InUScrollRate, const float InVScrollRate, const float InUScale, const float InVScale)
{
	if (InLayer < NUM_SKY_SHADER_LAYERS)
	{
		SetVec4(m_CloudScrollParams[InLayer], InUScrollRate, InVScrollRate, InUScale, InVScale);
	}
}

void EARS::Godfather::GlobalSkyData::SetAlphaScrollParams(const uint32_t InLayer, const float InUScrollRate, const float InVScrollRate, const float InUScale, const float InVScale)
{
	if (InLayer < NUM_SKY_SHADER_LAYERS)
	{
		SetVec4(m_AlphaScrollParams[InLayer], InUScrollRate, InVScrollRate, InUScale, InVScale);
	}
}

void EARS::Godfather::GlobalSkyData::SetTintColor(const uint32_t InLayer, const float InColor[4])
{
	if (InLayer < NUM_SKY_SHADER_LAYERS)
	{
		CopyVec4(m_TintColor[InLayer], InColor);
	}
}

uint32_t EARS::Godfather::GlobalSkyData::GetCloudTexture(const uint32_t InLayer) const
{
	return InLayer < NUM_SKY_SHADER_LAYERS ? m_CloudTexture[InLayer] : 0;
}

uint32_t EARS::Godfather::GlobalSkyData::GetAlphaTexture(const uint32_t InLayer) const
{
	return InLayer < NUM_SKY_SHADER_LAYERS ? m_AlphaTexture[InLayer] : 0;
}

void EARS::Godfather::GlobalSkyData::SetCloudTexture(const uint32_t InLayer, const uint32_t InTexture)
{
	if (InLayer < NUM_SKY_SHADER_LAYERS)
	{
		m_CloudTexture[InLayer] = InTexture;
	}
}

void EARS::Godfather::GlobalSkyData::SetAlphaTexture(const uint32_t InLayer, const uint32_t InTexture)
{
	if (InLayer < NUM_SKY_SHADER_LAYERS)
	{
		m_AlphaTexture[InLayer] = InTexture;
	}
}

void EARS::Godfather::GlobalSkyData::RestoreDefaults()
{
	for (uint32_t Layer = 0; Layer < NUM_SKY_SHADER_LAYERS; ++Layer)
	{
		SetVec4(m_CloudScrollParams[Layer], 0.1f, 0.1f, 1.0f, 1.0f);
		SetVec4(m_AlphaScrollParams[Layer], 0.1f, 0.1f, 1.0f, 1.0f);
		SetVec4(m_TintColor[Layer], 1.0f, 1.0f, 1.0f, 1.0f);

		m_CloudTexture[Layer] = 0;
		m_AlphaTexture[Layer] = 0;
	}

	SetVec4(m_SunDirAndPower, -0.684f, -0.25f, -0.684f, 0.41f);
	SetVec4(m_DiffusionControl, 0.05f, 350.0f, 11.5f, 12.5f);

	SetVec4(m_DiffusionColor, 0.3f, 0.2f, 0.2f, 1.0f);
	SetVec4(m_DiffusionColorLow, 0.5f, 0.3f, 0.2f, 1.0f);
	SetVec4(m_SkyColor, 0.5f, 0.5f, 1.0f, 1.0f);
	SetVec4(m_SkyColorLow, 0.8f, 0.8f, 1.0f, 1.0f);

	m_SkyDiffusionExp = 100.0f;
	m_SkyDiffusionExpLow = 10.0f;
	m_HazeBandAngle = 0.2f;
	m_CloudOpaqueDensity = 1.0f;
}
