#pragma once

// RenderWare Framework
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

// CPP
#include <windows.h>

/**
 * ImGui Manager for the Scripthook
 * (TOOD: Eventually this is going to register to tick, render etc)
 */
class ImGuiManager : public RWS::CEventHandler
{
public:

	ImGuiManager();

	//~ Begin RWS::CEventHandler Interface
	virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
	//~ End RWS::CEventHandler Interface

	/**
	 * Called when the manager needs to be initialised
	 */
	void Open();

	/**
	 * Called when the rendering is completed
	 */
	void OnEndScene();

	/**
	 * Does ImGui currently have cursor control
	 * @return bool - Whether or not it has cursor control
	 */
	bool HasCursorControl() const;

private:

	void DrawTab_PlayerModelSwap();

	void DrawTab_PlayerSettings();

	void DrawTab_TimeOfDaySettings();

	void DrawTab_DemographicSettings();

	void DrawTab_CitiesSettings();

	void DrawTab_PlayerFamilyTreeSettings();

	void DrawTab_UIHUDSettings();

	// Called when iMsgRunningTick event is detected
	void OnTick();

	// Should we render the ImGui demo window
	bool bShowImGuiDemoWindow = false;

	// Should we render the Parted Model window
	bool bShowModMenuWindow = false;

	// Should we enter a state where we take control of the Cursor?
	// In this state, we disable Player inputs, and get ImGui to visualise a cursor.
	bool bTakeoverCursor = false;

	bool bPlayerFlyModeActive = false;

	bool bPlayerGodModeActive = false;

	bool bPlayerVehicleGodModeActive = false;

	bool bWantsUISuppressed = false;
};
