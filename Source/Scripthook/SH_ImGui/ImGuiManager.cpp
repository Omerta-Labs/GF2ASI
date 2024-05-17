#include "ImGuiManager.h"

#include "Addons/Hook.h"

ImGuiManager::ImGuiManager()
	: CEventHandler()
{

}

void ImGuiManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	if (MsgEvent.IsEvent(RunningTickEvent))
	{
		int z = 0;
	}
}

static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
void ImGuiManager::Open()
{
	LinkMsg(&RunningTickEvent);
}

void ImGuiManager::Close()
{
	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	UnlinkMsg(&RunningTickEvent);
}
