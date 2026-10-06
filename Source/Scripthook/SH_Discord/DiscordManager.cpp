#include "DiscordManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"

// Godfather
#include "modules/turf/city.h"
#include "modules/turf/citymanager.h"

// CPP
#include <format>
#include <map>

namespace Events
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	static hook::Type<RWS::CEventId> PlayerAsDriverEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E030);
	static hook::Type<RWS::CEventId> PlayerExitVehicleEvent = hook::Type<RWS::CEventId>(0x112E018);
	static hook::Type<RWS::CEventId> CityChangedEvent = hook::Type<RWS::CEventId>(0x112DCF4);
	static hook::Type<RWS::CEventId> EnteredSelectMenuEvent = hook::Type<RWS::CEventId>(0x1130378);
	static hook::Type<RWS::CEventId> ExitedSelectMenuEvent = hook::Type<RWS::CEventId>(0x1130218);
	static hook::Type<RWS::CEventId> ChaseStartedEvent = hook::Type<RWS::CEventId>(0x112FE64);
	static hook::Type<RWS::CEventId> ChaseEndedEvent = hook::Type<RWS::CEventId>(0x112FE88);
	static hook::Type<RWS::CEventId> ExtortionSuccessCompleteEvent = hook::Type<RWS::CEventId>(0x112A658);
	static hook::Type<RWS::CEventId> PlayerLostVenueEvent = hook::Type<RWS::CEventId>(0x112A594);
	static hook::Type<RWS::CEventId> EnteredPauseMapEvent = hook::Type<RWS::CEventId>(0x11302B8);
	static hook::Type<RWS::CEventId> ExitedPauseMapEvent = hook::Type<RWS::CEventId>(0x1130248);
}

namespace Presence
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
		UpdateState("Cruising around");
	}

	if (MsgEvent.IsEvent(Events::PlayerExitVehicleEvent))
	{
		UpdateState("Walking around");
	}

	if (MsgEvent.IsEvent(Events::CityChangedEvent))
	{
		if (const EARS::Modules::CityManager* const CityMgr = EARS::Modules::CityManager::GetInstance())
		{
			const uint32_t CurrentCityID = CityMgr->GetCurrentCity();
			if (const String* DisplayName = CityMgr->GetDisplayName(CurrentCityID))
			{
				currentCity = DisplayName->c_str();
				m_CurrentActivity.GetAssets().SetLargeImage(Presence::GetSmallImageFromCityID(CurrentCityID));
				UpdateState("Walking around");
			}
		}
	}

	if (MsgEvent.IsEvent(Events::EnteredSelectMenuEvent))
	{
		UpdateDetails("Game paused");
	}

	if (MsgEvent.IsEvent(Events::ChaseStartedEvent))
	{
		UpdateDetails("Escaping");
	}

	if (MsgEvent.IsEvent(Events::ChaseEndedEvent))
	{
		UpdateDetails("Escaped");
	}

	if (MsgEvent.IsEvent(Events::ExtortionSuccessCompleteEvent))
	{
		UpdateDetails("Took over the racket");
	}

	if (MsgEvent.IsEvent(Events::PlayerLostVenueEvent))
	{
		UpdateDetails("The venue was stolen");
	}

	if (MsgEvent.IsEvent(Events::EnteredPauseMapEvent))
	{
		UpdateDetails("Planning the next move");
	}

	if (MsgEvent.IsEvent(Events::ExitedPauseMapEvent) || MsgEvent.IsEvent(Events::ExitedSelectMenuEvent))
	{
		UpdateDetails("Thinking like a don");
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

	assert(m_Core && "Discord Manager should have a m_Core to update state");
	m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {});

	C_Logger::Printf("Discord Initialised Successfully!");
}

void DiscordManager::OpenLevelServices()
{
	// apply more events
	// We can only apply if discord is active
	if (m_Core)
	{
		LinkMsg(Events::RunningTickEvent);
		LinkMsg(Events::PlayerAsDriverEnterVehicleEvent);
		LinkMsg(Events::PlayerExitVehicleEvent);
		LinkMsg(Events::CityChangedEvent);
		LinkMsg(Events::EnteredSelectMenuEvent);
		LinkMsg(Events::ExitedSelectMenuEvent);
		LinkMsg(Events::ChaseStartedEvent);
		LinkMsg(Events::ChaseEndedEvent);
		LinkMsg(Events::ExtortionSuccessCompleteEvent);
		LinkMsg(Events::PlayerLostVenueEvent);
		LinkMsg(Events::EnteredPauseMapEvent);
		LinkMsg(Events::ExitedPauseMapEvent);
	}
}

void DiscordManager::CloseLevelServices()
{
	// remove other events
	UnLinkMsg(Events::RunningTickEvent);
	UnLinkMsg(Events::PlayerAsDriverEnterVehicleEvent);
	UnLinkMsg(Events::PlayerExitVehicleEvent);
	UnLinkMsg(Events::CityChangedEvent);
	UnLinkMsg(Events::EnteredSelectMenuEvent);
	UnLinkMsg(Events::ExitedSelectMenuEvent);
	UnLinkMsg(Events::ChaseStartedEvent);
	UnLinkMsg(Events::ChaseEndedEvent);
	UnLinkMsg(Events::ExtortionSuccessCompleteEvent);
	UnLinkMsg(Events::PlayerLostVenueEvent);
	UnLinkMsg(Events::EnteredPauseMapEvent);
	UnLinkMsg(Events::ExitedPauseMapEvent);
}

void DiscordManager::UpdateState(std::string text)
{
	assert(m_Core && "Discord Manager must be enabled to update state!");

	const std::string NewState = std::format("{} {}", text, currentCity);
	m_CurrentActivity.SetState(NewState.data());
	m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {});
}

void DiscordManager::UpdateDetails(std::string text)
{
	assert(m_Core && "Discord Manager must be enabled to update activity details!");

	m_CurrentActivity.SetDetails(text.data());
	m_Core->ActivityManager().UpdateActivity(m_CurrentActivity, [](discord::Result result) {});
}

void DiscordManager::OnTick()
{
	assert(m_Core && "Discord Manager must be enabled to tick");

	m_Core->RunCallbacks();
}
