#include "DiscordManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"

// CPP
#include <format>
#include <string>
#include <map>

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
			return CityIDToImageAsset.at(CityID);
		}

		return nullptr;
	}
}

DiscordManager::DiscordManager()
	: CEventHandler()
{
}

DiscordManager::~DiscordManager()
{
	delete m_Core;

	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	UnlinkMsg(&RunningTickEvent);
}

void DiscordManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	RWS::CEventHandler::HandleEvents(MsgEvent);

	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	if (MsgEvent.IsEvent(RunningTickEvent))
	{
		OnTick();
	}
}

void DiscordManager::Open()
{
	auto result = discord::Core::Create(556346460850094100, DiscordCreateFlags_NoRequireDiscord, &m_Core);
	if (result != discord::Result::Ok || m_Core == nullptr)
	{
		C_Logger::Printf("Failed to initialise Discord. Error Code: %u", result);
		return;
	}

	// We should be okay to try and submit initial activity
	m_CurrentActivity.SetState("Playing Godfather II");
	m_CurrentActivity.SetDetails("Thinking like a don");
	m_CurrentActivity.GetAssets().SetLargeImage("main");
	m_CurrentActivity.GetTimestamps().SetStart(discord::Timestamp(std::time(0)));

	m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {});

	// Initialised too, we can accept tick
	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	LinkMsg(&RunningTickEvent, 0x8000);

	C_Logger::Printf("Discord Initialised Successfully!");
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
				m_CurrentActivity.SetState(NewState.data());
				m_CurrentActivity.GetAssets().SetSmallImage(Precense::GetSmallImageFromCityID(uCurrentCityID));

				// push new activity
				m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {
					});
			}
		}
	}

	m_Core->RunCallbacks();
}
