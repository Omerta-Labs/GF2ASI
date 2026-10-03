#include "ShadowHelper.h"

float EARS::ShadowHelper::GetDirLightShadowDistance(const uint32_t InCascade)
{
	if (InCascade >= NUM_CASCADED_SHADOW_MAPS)
	{
		return 0.0f;
	}

	return reinterpret_cast<float*>(ADDR_DIR_LIGHT_SHADOW_DISTANCES)[InCascade];
}

void EARS::ShadowHelper::SetDirLightShadowDistance(const uint32_t InCascade, const float InDistance)
{
	if (InCascade >= NUM_CASCADED_SHADOW_MAPS)
	{
		return;
	}

	reinterpret_cast<float*>(ADDR_DIR_LIGHT_SHADOW_DISTANCES)[InCascade] = InDistance;
}

float EARS::ShadowHelper::GetDirLightShadowHeight()
{
	return *reinterpret_cast<float*>(ADDR_DIR_LIGHT_SHADOW_HEIGHT);
}

void EARS::ShadowHelper::SetDirLightShadowHeight(const float InHeight)
{
	*reinterpret_cast<float*>(ADDR_DIR_LIGHT_SHADOW_HEIGHT) = InHeight;
}

float EARS::ShadowHelper::GetCharShadowVertRangeMin()
{
	return *reinterpret_cast<float*>(ADDR_CHAR_SHADOW_VERT_RANGE_MIN);
}

float EARS::ShadowHelper::GetCharShadowVertRangeMax()
{
	return *reinterpret_cast<float*>(ADDR_CHAR_SHADOW_VERT_RANGE_MAX);
}

void EARS::ShadowHelper::SetCharShadowVertRange(const float InMin, const float InMax)
{
	*reinterpret_cast<float*>(ADDR_CHAR_SHADOW_VERT_RANGE_MIN) = InMin;
	*reinterpret_cast<float*>(ADDR_CHAR_SHADOW_VERT_RANGE_MAX) = InMax;
}
