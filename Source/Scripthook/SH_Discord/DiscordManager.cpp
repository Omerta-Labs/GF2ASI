#include "DiscordManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/discord/discord.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"

// CPP
#include <format>
#include <string>
#include <map>

discord::Core* core{};
discord::Activity activity{};

namespace Precense
{
	// Hardcoded list for image icons
	// We could probably eventually switch this to include venues too.
	static const std::map<uint32_t, const char*> CityIDToImageAsset =
	{
		{ 0x8DA8826F, "new_york"},
		{ 0xF72A95D, "florida"},
		{ 0x3821E851, "cuba"}
	};

	// A utility function to fetch the image based on CityID. 
	const char* GetSmallImageFromCityID(const uint32_t CityID)
	{
		if (CityIDToImageAsset.contains(CityID))
		{
			return CityIDToImageAsset[CityID];
		}

		return nullptr;
	}
}

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
	if (const EARS::Modules::CityManager* const CityMgr = EARS::Modules::CityManager::GetInstance())
	{
		const uint32_t NewCityID = CityMgr->GetCurrentCity();
		if (uCurrentCityID != NewCityID)
		{
			uCurrentCityID = NewCityID;
			if (const String* DisplayName = CityMgr->GetDisplayName(NewCityID))
			{
				// update state to represent new city
				const std::string NewState = std::format("Visiting {}", DisplayName->m_pCStr);
				activity.SetState(NewState.data());
				activity.GetAssets().SetSmallImage(Precense::GetSmallImageFromCityID(uCurrentCityID));

				// push new activity
				core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
					});

				// triggered update request
				::core->RunCallbacks();
			}
		}
	}
}
