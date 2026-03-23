#include "SDK/EARS_RT_CCT/include/ChrCntrl_Extensions.h"

#include "SDK/EARS_RT_CCT/include/ChrCntrl_AnimView.h"

/* static */
bool ChrCntl_Extensions_AnimEnd(EA::CCT::AnimView* pAnimViewInfo)
{
	// TODO: Identify flags
	constexpr int32_t FLAGS_TO_TEST = 0x2000003;
	if (pAnimViewInfo->GetNumFrames() <= 1.0f || pAnimViewInfo->TestFlags(FLAGS_TO_TEST))
	{
		return true;
	}

	return false;
}
