#pragma once

// RenderWare Framework
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

class ImGuiManager : public RWS::CEventHandler
{
public:

	ImGuiManager();

	//~ Begin RWS::CEventHandler Interface
	virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
	//~ End RWS::CEventHandler Interface

	void Open();
	void Close();

private:
};
