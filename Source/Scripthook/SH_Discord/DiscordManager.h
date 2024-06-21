#pragma once

// RenderWare Framework
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

// CPP
#include <ctime>

/**
 * Discord Manager for the Scripthook
 */
class DiscordManager : public RWS::CEventHandler
{
public:

	DiscordManager();

	//~ Begin RWS::CEventHandler Interface
	virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
	//~ End RWS::CEventHandler Interface

	/**
	 * Called when the manager needs to be initialised
	 */
	void Open();

private:

	// Called when iMsgRunningTick event is detected
	void OnTick();

	uint32_t uCurrentCityID = 0;
};
