#include "GodfatherShadows.h"

// SDK Framework
#include "SDK/ears_framework/src/framework/core/graphics/ShadowHelper.h"

namespace
{
	// Singleton<EARS::Modules::GodfatherShadowManager> instance pointer
	constexpr uintptr_t ADDR_GODFATHER_SHADOW_MANAGER = 0x11318E4;
}

EARS::Modules::GodfatherShadowManager* EARS::Modules::GodfatherShadowManager::GetInstance()
{
	return *reinterpret_cast<GodfatherShadowManager**>(ADDR_GODFATHER_SHADOW_MANAGER);
}

void EARS::Modules::GodfatherShadowManager::RestoreDefaultShadowParams()
{
	if (m_pShadowParams == nullptr)
	{
		return;
	}

	m_pShadowParams->m_Range = EARS::ShadowHelper::GetDirLightShadowDistance(0) * 0.5f;
	m_pShadowParams->m_Height = 100.0f;
	m_pShadowParams->m_Flags = 0;
}
