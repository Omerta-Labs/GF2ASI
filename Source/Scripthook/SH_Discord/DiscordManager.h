#pragma once

// RenderWare Framework
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/CEventHandler.h"

// Addons
#include "Addons/discord/discord.h"

// Scripthook
#include "Utils/Singleton.h"

// CPP
#include <ctime>
#include <string>

/**
 * Discord Manager for the Scripthook
 */
class DiscordManager : public RWS::CEventHandler, public SH::Singleton<DiscordManager>
{
public:

	DiscordManager();
	virtual ~DiscordManager();

	//~ Begin RWS::CEventHandler Interface
	virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
	//~ End RWS::CEventHandler Interface

	/**
	 * Called when the manager needs to be initialised
	 */
	void Open();

	/**
	 * Update manager when level services become active
	 */
	void OpenLevelServices();

	/**
	 * Update manager when level services close.
	 */
	void CloseLevelServices();

private:

	void UpdateState(std::string);

	void UpdateDetails(std::string);

	// Called when iMsgRunningTick event is detected
	void OnTick();

	std::string currentCity = "New York";

	discord::Core* m_Core = nullptr;
	discord::Activity m_CurrentActivity{};
};
