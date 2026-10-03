#include "SDK/ears_rt_cct/include/ears_rt_cct/highlevel/chrcntl_extensions.h"

#include "SDK/ears_rt_cct/include/ears_rt_cct/chrcntl_animview.h"

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
