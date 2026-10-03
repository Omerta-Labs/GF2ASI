#pragma once

// addons
#include "Scripthook/SH_ImGui/ImGuiNPCInspector.h"
#include "Scripthook/SH_ImGui/ImGuiCheckpointDebug.h"
#include "Scripthook/SH_ImGui/ImGuiPhotoModeSystem.h"
#include "Scripthook/SH_ImGui/ImGuiUISystem.h"
#include "Utils/Singleton.h"

// RenderWare Framework
#include "framework/core/eventhandler/ceventhandler.h"

// Common
#include "ears_common/guid.h"
#include "ears_common/rwtypes.h"

// ImGui
#include "Addons/imgui/imgui.h"

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
	 * Whether the menu takes input while it is on screen. Turning this off leaves the
	 * menu visible but hands the mouse and keyboard back to the game, so the player
	 * keeps their camera controls.
	 */
	bool IsMenuInteractive() const { return bImGuiInteractive; }
	void SetMenuInteractive(bool bInteractive) { bImGuiInteractive = bInteractive; }

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

private:

	struct BuildingTeleportPayload
	{
		RwV3d TeleportLocation;
	};

	// Registered with Mod::ImGuiRuntime; forwards to DrawMenu on the instance.
	static void DrawMenuPanel();

	// Draws the menu window and the inspector. Runs inside the runtime's frame.
	void DrawMenu();

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

	// Whether the mod menu takes the mouse and keyboard when it is on screen.
	// Turning this off leaves the menu visible but hands input back to the game,
	// so live readouts can be watched while playing. Defaults on, otherwise
	// opening the menu would present a window that cannot be clicked.
	bool bImGuiInteractive = true;

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

};
