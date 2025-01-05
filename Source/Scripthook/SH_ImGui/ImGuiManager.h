#pragma once

// addons
#include "Scripthook/SH_ImGui/ImGuiNPCInspector.h"

// RenderWare Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Common
#include "SDK/EARS_Common/Guid.h"

// CPP
#include <windows.h>
#include <string>
#include <vector>

// Forward declarations
namespace EARS
{
	namespace Vehicles
	{
		class WhiteboxCar;
	}
}

/**
 * ImGui Manager for the Scripthook
 */
class ImGuiManager : public RWS::CEventHandler
{
public:

	ImGuiManager();
	virtual ~ImGuiManager();

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

	/**
	 * API for ImGui to listen for Windows messages
	 * Do not call outside of a WndProc function handler!
	 */
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	struct EntityEntry
	{
		std::string Name;
		EARS::Common::guid128_t GUID;
	};

	void DrawTab_PlayerSettings();

	void DrawTab_CheckpointSettings();

	void DrawTab_TimeOfDaySettings();

	void DrawTab_DemographicSettings();

	void DrawTab_CitiesSettings();

	void DrawTab_PlayerFamilyTreeSettings();

	void DrawTab_UIHUDSettings();

	void DrawTab_ObjectMgrSettings();

	void DrawTab_Support();

	bool SetVehicleGodMode(EARS::Vehicles::WhiteboxCar* InVehicle, bool bGodModeActive) const;

	// Load a list of Vehicle GUIDs from the specified text documents; deals with duplication automatically
	void LoadEntityGuidsFromFile(const std::string& Filename, std::vector<EntityEntry>& OutVector) const;

	// Initialise an NPC Inspector for a given object in the game world
	void InitialiseNPCInspector(EARS::Modules::Sentient* InSentient, const bool bIsPlayer);

	// Called when iMsgRunningTick event is detected
	void OnTick();
	
	// List of spawnable vehicle guids
	std::vector<EntityEntry> VehicleEntries;
	EARS::Common::guid128_t SelectedVehicleGuid;

	// List of spawnable NPC guids
	std::vector<EntityEntry> NPCEntries;
	EARS::Common::guid128_t SelectedNPCGuid;

	// Inspector for the current object
	// (Either Player or NPC)
	ImGuiNPCInspector CurrentInspector;

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
