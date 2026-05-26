#pragma once

// addons
#include "Scripthook/SH_ImGui/ImGuiNPCInspector.h"
#include "Scripthook/SH_ImGui/ImGuiCheckpointDebug.h"
#include "Utils/Singleton.h"

// RenderWare Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Common
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/RwMaths.h"

// CPP
#include <windows.h>
#include <string>
#include <optional>

// Forward declarations
namespace EARS
{
	namespace Modules
	{
		class Family;
	}

	namespace Vehicles
	{
		class WhiteboxCar;
	}
}

/**
 * ImGui Manager for the Scripthook
 */
class ImGuiManager : public RWS::CEventHandler, public SH::Singleton<ImGuiManager>
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
	 * Update manager when level services become active
	 */
	void OpenLevelServices();

	/**
	 * Update manager when level services close.
	 */
	void CloseLevelServices();

	/** Fetch Checkpoint tab debug*/
	SH::ImGuiCheckpointDebug& GetCheckpointDebug() { return CheckpointDebug; }

	/**
	 * API for ImGui to listen for Windows messages
	 * Do not call outside of a WndProc function handler!
	 */
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	struct BuildingTeleportPayload
	{
		RwV3d TeleportLocation;
	};

	void DrawTab_PlayerSettings();

	void DrawTab_CheckpointSettings();

	void DrawTab_TimeOfDaySettings();

	void DrawTab_DemographicSettings();

	void DrawTab_CitiesSettings();

	void DrawTab_BuildingSettings();

	void DrawTab_FamiliesSettings();

	void DrawTab_PlayerFamilyTreeSettings();

	void DrawTab_UIHUDSettings();

	void DrawTab_ObjectMgrSettings();

	void DrawTab_SimMgrSettings();

	void DrawTab_Support();

	bool SetVehicleGodMode(EARS::Vehicles::WhiteboxCar* InVehicle, bool bGodModeActive) const;

	// Initialise an NPC Inspector for a given object in the game world
	void InitialiseNPCInspector(EARS::Modules::Sentient* InSentient, const bool bIsPlayer);

	void ProcessBuildingTeleport();

	// Called when iMsgRunningTick event is detected
	void OnTick();
	
	// Inspector for the current object
	// (Either Player or NPC)
	ImGuiNPCInspector CurrentInspector;

	SH::ImGuiCheckpointDebug CheckpointDebug;

	std::optional<BuildingTeleportPayload> DeferredTeleportPayload;

	// Should we render the Parted Model window
	bool bShowModMenuWindow = false;

	// Whether or not any ImGui windows are interactive
	bool bImGuiInteractive = false;

	// Should we enter a state where we take control of the Cursor?
	// In this state, we disable Player inputs, and get ImGui to visualise a cursor.
	bool bTakeoverCursor = false;

	bool bPlayerGodModeActive = false;

	bool bPlayerVehicleGodModeActive = false;

	bool bWantsUISuppressed = false;

	std::string InventoryAddItem_SelectedName;
	EARS::Common::guid128_t InventoryAddItem_SelectedGuid;

	// TODO: Does this need SafePtr? WeakPtr?
	EARS::Modules::Family* TargetFamily = nullptr;
};
