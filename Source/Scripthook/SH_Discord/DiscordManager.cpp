#include "DiscordManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/discord/discord.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"

// CPP
#include <string>

discord::Core* core{};
discord::Activity activity{};

DiscordManager::DiscordManager()
	: CEventHandler()
{

}

void DiscordManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	if (MsgEvent.IsEvent(RunningTickEvent))
	{
		OnTick();
	}
}

void DiscordManager::Open()
{
	auto result = discord::Core::Create(556346460850094100, DiscordCreateFlags_NoRequireDiscord, &core);
	activity.SetState("Playing Godfather II");
	activity.SetDetails("Thinking like a don");
	activity.GetAssets().SetLargeImage("main");
	activity.GetTimestamps().SetStart(discord::Timestamp(std::time(0)));
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}

void DiscordManager::OnTick()
{
	if (EARS::Modules::CityManager* CityMgr = EARS::Modules::CityManager::GetInstance())
	{
		const uint32_t NewCityID = CityMgr->GetCurrentCity();
		if (uCurrentCityID != NewCityID)
		{
			uCurrentCityID = NewCityID;
			String* DisplayName = CityMgr->GetDisplayName(NewCityID);
			std::string Sentence = "Living in ";
			const char *CityName = DisplayName->m_pCStr;
			Sentence += CityName;
			activity.SetState(Sentence.c_str());
			core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
	}
	::core->RunCallbacks();
}
