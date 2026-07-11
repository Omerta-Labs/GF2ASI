#pragma once

// addons
#include "Scripthook/SH_ImGui/ImGuiNPCInspector.h"
#include "Scripthook/SH_ImGui/ImGuiCheckpointDebug.h"
#include "Scripthook/SH_ImGui/ImGuiPhotoModeSystem.h"
#include "Scripthook/SH_ImGui/ImGuiUISystem.h"
#include "Utils/Singleton.h"

// RenderWare Framework
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Common
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/RwMaths.h"

// ImGui
#include "Addons/imgui/imgui.h"

// CPP
#include <windows.h>
#include <mutex>
#include <string>
#include <optional>

// Forward declarations
namespace EARS
{
	namespace Modules
	{
		class Family;
		class MarketingCameraInfo;
		class Player;
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
	 * Called when the rendering is completed.
	 * Runs on the PRESENTATION thread - only renders the draw data snapshot,
	 * never the live ImGui context (the SIM thread may be rebuilding it).
	 */
	void OnEndScene();

	/**
	 * Called on the presentation thread before the D3D9 device is reset.
	 * Releases ImGui device objects and drops the draw data snapshot, which
	 * may reference textures that are about to be destroyed.
	 */
	void OnDeviceLost();

	/**
	 * Called on the presentation thread after a successful D3D9 device reset.
	 */
	void OnDeviceRestored();

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
	SH::ImGuiUISystem& GetUISystemDebug() { return UISystem; }

	static SH::ImGuiCheckpointDebug& StaticGetCheckpointDebug();
	static SH::ImGuiUISystem& StaticGetUISystemDebug();

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

	void DrawTab_PhotoMode();

	void DrawTab_TimeOfDaySettings();

	void DrawTab_DemographicSettings();

	void DrawTab_CitiesSettings();

	void DrawTab_BuildingSettings();

	void DrawTab_FamiliesSettings();

	void DrawTab_PlayerFamilyTreeSettings();

	void DrawTab_ObjectMgrSettings();

	void DrawTab_SimMgrSettings();

	void DrawTab_Support();

	// Table of every registered shortcut with in-menu rebinding controls
	void DrawTab_Keybinds();

	// Register the mod's shortcut actions with the KeybindManager and load their
	// bindings from the ini. Called once from Open().
	void RegisterShortcutActions();

	void SetPlayerGodMode(EARS::Modules::Player& InPlayer) const;

	bool SetVehicleGodMode(EARS::Vehicles::WhiteboxCar* InVehicle, bool bGodModeActive) const;

	void SetPlayerFlyMode(bool bIsActive);

	void ToggleFreezeLogic();

	// Initialise an NPC Inspector for a given object in the game world
	void InitialiseNPCInspector(EARS::Modules::Sentient* InSentient, const bool bIsPlayer);

	void ProcessBuildingTeleport();

	// Called when iMsgRunningTick event is detected
	void OnTick();

	// Deep-copy the just-rendered frame's draw data into SnapshotDrawData.
	// Expects ImGuiContextLock to be held.
	void CaptureDrawDataSnapshot();

	// Free the cloned draw lists owned by SnapshotDrawData and mark it invalid.
	// Expects ImGuiContextLock to be held.
	void ClearDrawDataSnapshot();

	void AddFont(const char* name);

	// Inspector for the current object
	// (Either Player or NPC)
	ImGuiNPCInspector CurrentInspector;

	SH::ImGuiCheckpointDebug CheckpointDebug;

	SH::ImGuiPhotoModeSystem PhotoModeSystem;

	SH::ImGuiUISystem UISystem;

	std::optional<BuildingTeleportPayload> DeferredTeleportPayload;

	bool bShowImGuiStyleEditor = false;

	// Should we render the Parted Model window
	bool bShowModMenuWindow = false;

	// Whether or not any ImGui windows are interactive
	bool bImGuiInteractive = false;

	// Should we enter a state where we take control of the Cursor?
	// In this state, we disable Player inputs, and get ImGui to visualise a cursor.
	bool bTakeoverCursor = false;

	bool bPlayerGodModeActive = false;

	bool bPlayerVehicleGodModeActive = false;

	bool bFreezeLogic = false;

	std::string InventoryAddItem_SelectedName;
	EARS::Common::guid128_t InventoryAddItem_SelectedGuid;

	// TODO: Does this need SafePtr? WeakPtr?
	EARS::Modules::Family* TargetFamily = nullptr;

	// Serialises all ImGui context access between the SIM thread (frame build
	// in OnTick), the PRESENTATION thread (OnEndScene / device reset) and the
	// window thread (WndProc input events).
	// Recursive because the Win32 backend re-enters WndProc on the same thread:
	// its handler calls ReleaseCapture(), which synchronously dispatches
	// WM_CAPTURECHANGED back into the window proc while the lock is held.
	std::recursive_mutex ImGuiContextLock;

	// Deep copy of the last completed frame's draw data, owned by us (the
	// ImDrawLists are clones). The presentation thread renders this instead of
	// the live context, so it always sees a complete frame regardless of how
	// many presents happen per SIM tick.
	ImDrawData SnapshotDrawData;

	// Stored Fonts
	struct ImFont* CustomFont = nullptr;
	struct ImFont* DefaultFont = nullptr;

};
