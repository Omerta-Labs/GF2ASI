#include "Base.h"

namespace EARS::Framework
{
	bool Base::IsEventHandlerBase(const RWS::CEventHandler& InHandler)
	{
		return InHandler.GetEventHandlerFlags() & (uint32_t)CEventHandlerFlags::CEVENTHANDLER_FLAG_BASE;
	}
}
