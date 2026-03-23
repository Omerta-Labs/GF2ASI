#include "SDK/EARS_RT_CCT/include/ChrCntrl_Secondary.h"

#include "SDK/EARS_RT_CCT/include/ChrCntrl_AnimView.h"

// C++
#include <assert.h>

/* static */
static EA::CCT::AnimView* GetSecondaryView(EA::CCT::AnimView* InAnimView, int InSlot)
{
	if (!InAnimView || InSlot < 0)
	{
		return nullptr;
	}

	EA::CCT::AnimView* Secondary = InAnimView->GetSecondary();
	for (int i = 0; i < InSlot; ++i)
	{
		Secondary = Secondary->GetSecondary();
		if (Secondary == nullptr)
		{
			break;
		}
	}

	return Secondary;
}

/* static */
bool HasSecondaryAnimChain(EA::CCT::AnimView* InAnimView)
{
	assert(InAnimView);

	return (InAnimView->GetSecondary() != nullptr);
}

/* static */
int32_t ChrCntl_Secondary_GetCurrStateId(EA::CCT::AnimView* InAnimView, int InSlot)
{
	assert(InAnimView);
	assert(InSlot >= 0);

	if (EA::CCT::AnimView* SecondaryView = GetSecondaryView(InAnimView, InSlot))
	{
		return SecondaryView->GetStateId();
	}

	return -1;
}

/* static */
bool ChrCntl_Secondary_AnimEnd(EA::CCT::AnimView* InAnimView, int InSlot)
{
	assert(InAnimView);
	assert(InSlot >= 0);

	if (const EA::CCT::AnimView* SecondaryView = GetSecondaryView(InAnimView, InSlot))
	{
		// TODO: Identify flags
		constexpr int32_t FLAGS_TO_TEST = 0x2000003;
		if (SecondaryView->GetNumFrames() <= 1.0f || SecondaryView->TestFlags(FLAGS_TO_TEST))
		{
			return true;
		}
	}

	return false;
}
