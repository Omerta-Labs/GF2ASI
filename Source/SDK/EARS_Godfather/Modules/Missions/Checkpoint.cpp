#include "Checkpoint.h"

// addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	void Checkpoint::StopLoading()
	{
		static hook::Type<RWS::CEventId> iMsgStreamSetLoadCompleteEvent = hook::Type<RWS::CEventId>(0x120E970);
		UnlinkMsg(&iMsgStreamSetLoadCompleteEvent);

		m_bIsLoading = false;	
	}
}
