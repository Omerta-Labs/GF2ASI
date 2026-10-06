#include "checkpoint.h"

// addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	void Checkpoint::StopLoading()
	{
		static hook::Type<RWS::CEventId> iMsgStreamSetLoadCompleteEvent = hook::Type<RWS::CEventId>(0x120E970);
		UnLinkMsg(iMsgStreamSetLoadCompleteEvent);

		m_bIsLoading = false;	
	}
}
