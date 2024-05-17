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

	// Called when iMsgRunningTick event is detected
	void OnTick();

	// Virtual Key to show the ImGui Demo Window
	int ShowImGuiDemoWindowInput = VK_F2;

	// Virtual Key to show the Parted Model Window
	int ShowPartedModelWindowInput = VK_F1;

	// Should we render the ImGui demo window
	bool bShowImGuiDemoWindow = false;

	// Should we render the Parted Model window
	bool bShowPartedModelWindow = false;

	// Should we enter a state where we take control of the Cursor?
	// In this state, we disable Player inputs, and get ImGui to visualise a cursor.
	bool bTakeoverCursor = false;
};
