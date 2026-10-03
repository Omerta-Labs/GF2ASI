#include "RenderingParams.h"

namespace
{
	// Singleton<EARS::Framework::RenderingParams> instance pointer
	constexpr uintptr_t ADDR_RENDERING_PARAMS = 0x122348C;
}

EARS::Framework::RenderingParams* EARS::Framework::RenderingParams::GetInstance()
{
	return *reinterpret_cast<RenderingParams**>(ADDR_RENDERING_PARAMS);
}

void EARS::Framework::RenderingParams::SetGammaCorrection(const float InRed, const float InGreen, const float InBlue)
{
	m_GammaCorrection[0] = InRed;
	m_GammaCorrection[1] = InGreen;
	m_GammaCorrection[2] = InBlue;
}

void EARS::Framework::RenderingParams::RestoreDefaults()
{
	m_GammaCorrection[0] = 1.0f;
	m_GammaCorrection[1] = 1.0f;
	m_GammaCorrection[2] = 0.0f;
	m_GammaCorrection[3] = 0.0f;

	m_bApplyOnLoad = true;
}
