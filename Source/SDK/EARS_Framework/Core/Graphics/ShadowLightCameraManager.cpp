#include "ShadowLightCameraManager.h"

namespace
{
	// Singleton<EARS::ShadowLightCameraManager> instance pointer
	constexpr uintptr_t ADDR_SHADOW_LIGHT_CAMERA_MANAGER = 0x12233B0;
}

EARS::ShadowLightCameraManager* EARS::ShadowLightCameraManager::GetInstance()
{
	return *reinterpret_cast<ShadowLightCameraManager**>(ADDR_SHADOW_LIGHT_CAMERA_MANAGER);
}

EARS::Modules::EARSLight* EARS::ShadowLightCameraManager::GetShadowLight(const uint32_t InSlot) const
{
	return InSlot < NUM_SHADOW_CASTERS ? m_pShadowLights[InSlot] : nullptr;
}

void EARS::ShadowLightCameraManager::SetShadowLight(const uint32_t InSlot, EARS::Modules::EARSLight* InLight)
{
	if (InSlot < NUM_SHADOW_CASTERS)
	{
		m_pShadowLights[InSlot] = InLight;
	}
}

EARS::DirectionalShadowParamData* EARS::ShadowLightCameraManager::GetShadowParams(const uint32_t InSlot) const
{
	return InSlot < NUM_SHADOW_CASTERS ? m_pShadowParams[InSlot] : nullptr;
}

void EARS::ShadowLightCameraManager::SetShadowParams(const uint32_t InSlot, DirectionalShadowParamData* InParams)
{
	if (InSlot < NUM_SHADOW_CASTERS)
	{
		m_pShadowParams[InSlot] = InParams;
	}
}

void EARS::ShadowLightCameraManager::ClearShadowCasters()
{
	for (uint32_t Slot = 0; Slot < NUM_SHADOW_CASTERS; ++Slot)
	{
		m_pShadowLights[Slot] = nullptr;
		m_pShadowParams[Slot] = nullptr;
	}
}
