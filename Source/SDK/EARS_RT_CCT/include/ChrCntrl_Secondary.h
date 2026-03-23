#pragma once

// C++
#include <stdint.h>

// forward declares
namespace EA::CCT
{
	class AnimView;
}

/** True if a secondary anim chain exists (AnimView+0x124 != nullptr). */
extern bool HasSecondaryAnimChain(EA::CCT::AnimView* InAnimView);

extern int32_t ChrCntl_Secondary_GetCurrStateId(EA::CCT::AnimView* InAnimView, int InSlot);

extern bool ChrCntl_Secondary_AnimEnd(EA::CCT::AnimView* InAnimView, int InSlot);
