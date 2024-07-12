#include "DiscordManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"

// CPP
#include <format>
#include <map>

namespace Events
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	hook::Type<RWS::CEventId> PlayerAsDriverEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E030);
	hook::Type<RWS::CEventId> PlayerExitVehicleEvent = hook::Type<RWS::CEventId>(0x112E018);
	hook::Type<RWS::CEventId> CityChangedEvent = hook::Type<RWS::CEventId>(0x112DCF4);
}

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

	UnlinkMsg(&Events::RunningTickEvent);
	UnlinkMsg(&Events::PlayerAsDriverEnterVehicleEvent);
	UnlinkMsg(&Events::PlayerExitVehicleEvent);
	UnlinkMsg(&Events::CityChangedEvent);
}

void DiscordManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	RWS::CEventHandler::HandleEvents(MsgEvent);

	if (MsgEvent.IsEvent(Events::RunningTickEvent))
	{
		OnTick();
	}

	if (MsgEvent.IsEvent(Events::PlayerAsDriverEnterVehicleEvent))
	{
		UpdatePresence("Cruising around");
	}

	if (MsgEvent.IsEvent(Events::PlayerExitVehicleEvent))
	{
		UpdatePresence("Walking around");
	}

	if (MsgEvent.IsEvent(Events::CityChangedEvent))
	{
		if (const EARS::Modules::CityManager* const CityMgr = EARS::Modules::CityManager::GetInstance())
		{
			const uint32_t CurrentCityID = CityMgr->GetCurrentCity();
			if (const String* DisplayName = CityMgr->GetDisplayName(CurrentCityID))
			{
				currentCity = DisplayName->m_pCStr;
				m_CurrentActivity.GetAssets().SetLargeImage(Precense::GetSmallImageFromCityID(CurrentCityID));
				UpdatePresence("Walking around");
			}
		}
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
	m_CurrentActivity.GetAssets().SetSmallImage("main");
	m_CurrentActivity.GetTimestamps().SetStart(discord::Timestamp(std::time(0)));

	m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {});

	LinkMsg(&Events::RunningTickEvent, 0x8000);
	LinkMsg(&Events::PlayerAsDriverEnterVehicleEvent, 0x8000);
	LinkMsg(&Events::PlayerExitVehicleEvent, 0x8000);
	LinkMsg(&Events::CityChangedEvent, 0x8000);

	C_Logger::Printf("Discord Initialised Successfully!");
}

void DiscordManager::UpdatePresence(std::string text)
{
	const std::string NewState = std::format("{} {}", text, currentCity);
	m_CurrentActivity.SetState(NewState.data());
	m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {});
}

void DiscordManager::OnTick()
{
	m_Core->RunCallbacks();
}
