#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tConsole.h"
#include "Addons/tLog.h"
#include "Addons/Settings.h"
#include "Addons/ImGuiRuntime.h"
#include "Scripthook/SH_ObjectManager/ObjectManager.h"
#include "Scripthook/SH_ImGui/GameEventIds.h"
#include "Addons/KeybindManager.h"

// Godfather
#include "framework/core/camera/cameramanager.h"
#include "framework/core/graphics/materialhash.h"
#include "framework/core/graphics/materialmanager.h"
#include "framework/core/simmanager/simmanager.h"
#include "framework/core/streammanager/streammanager.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/group/groupmanager.h"
#include "modules/buildings/building.h"
#include "modules/buildings/building_store.h"
#include "modules/buildings/building_manager.h"
#include "modules/families/family.h"
#include "modules/families/family_manager.h"
#include "modules/families/corleone_data.h"
#include "modules/families/mademan.h"
#include "modules/item/inventorymanager.h"
#include "modules/player/player.h"
#include "modules/player/playerdebug.h"
#include "modules/mobface/mobfacemanager.h"
#include "modules/timeofday/timeofdaymanager.h"
#include "modules/turf/city.h"
#include "modules/turf/citymanager.h"
#include "modules/npc/npc.h"
#include "modules/npc/crime/crimemanager.h"
#include "modules/npcscheduling/demographicregion.h"
#include "modules/npcscheduling/demographicregionmanager.h"
#include "modules/npcscheduling/simnpc.h"
#include "modules/vehicles/behaviors/whitebox_car/whitebox_car.h"
#include "modules/vehicles/vehicledamagecomponent.h"
#include "ears_physics/characters/characterproxy.h"
#include "ears_physics/vehicles/ground/wheeled/havok_wheeled_vehicle.h"

#include "ears_rt_llrender/shadermanager.h"

// CPP
#include <filesystem>
#include <string>

#define ENABLE_ENTITY_SPAWN_DEBUG 0

#define SHOW_ATTRIBUTEPACKET_WINDOW 1

#if DEBUG
#define SHOW_DEMOGRAPHICS_TAB 0
#define SHOW_FAMILY_TAB 1
#else
#define SHOW_DEMOGRAPHICS_TAB 0
#define SHOW_FAMILY_TAB 1
#endif // DEBUG

#if ENABLE_ENTITY_SPAWN_DEBUG
class NPCManager
{
public:

	void* Create(const EARS::Common::guid128_t& InGuid, uint32_t InPriority, void* InOwner, uint32_t InHStream)
	{
		return MemUtils::CallClassMethod<void*, NPCManager*, const EARS::Common::guid128_t&, uint32_t, void*, uint32_t>(
			0x08F0BB0, this, InGuid, InPriority, InOwner, InHStream);
	}

	static NPCManager* GetInstance()
	{
		// 
		return *(NPCManager**)0x112FDD4;
	}
};
#endif // ENABLE_ENTITY_SPAWN_DEBUG


ImGuiManager::ImGuiManager()
	: CEventHandler()
{

}

ImGuiManager::~ImGuiManager()
{
	Mod::ImGuiRuntime::Close();
}

void ImGuiManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	RWS::CEventHandler::HandleEvents(MsgEvent);

	if (MsgEvent.IsEvent(DefinedEvents::RunningTickEvent) 
	|| MsgEvent.IsEvent(DefinedEvents::PausedTickEvent))
	{
		OnTick();
	}
	else if (MsgEvent.IsEvent(DefinedEvents::iMsgPlayerTeleportDoneExceptFade))
	{
		ProcessBuildingTeleport();

		UnlinkMsg(&DefinedEvents::iMsgPlayerTeleportDoneExceptFade);
	}
	else if (MsgEvent.IsEvent(DefinedEvents::PlayerExitVehicleEvent))
	{
		if (bPlayerVehicleGodModeActive)
		{
			// try and disable god mode
			if (const EARS::Modules::Player* const LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
			{
				SetVehicleGodMode(LocalPlayer->GetVehicle(), false);
				bPlayerVehicleGodModeActive = false;
			}
		}
	}
}

void ImGuiManager::Open()
{
	if (!Mod::ImGuiRuntime::Open())
	{
		// InitialiseHook spins until the D3D9 device exists before calling
		// Init_GameSystems, so this should not happen. Nothing retries it, so
		// the menu would be absent for the session.
		tConsole::fPrintf("ImGuiManager::Open: runtime refused to open, no menu this session");
		return;
	}

	Mod::ImGuiRuntime::RegisterPanel(&ImGuiManager::DrawMenuPanel);

	RegisterShortcutActions();
}

void ImGuiManager::RegisterShortcutActions()
{
	SH::KeybindManager& Keybinds = *SH::KeybindManager::Get();

	// Menu shortcuts are always available; gameplay shortcuts guard on the local
	// player so a stray key press at the front end is a harmless no-op.
	Keybinds.RegisterAction({ "menu.toggle", "Toggle Mod Menu", "Menu", VK_F1,
		[this]() { bShowModMenuWindow = !bShowModMenuWindow; },
		[this]() { return bShowModMenuWindow; } });

	Keybinds.RegisterAction({ "menu.interactive", "Toggle Cursor Interaction", "Menu", VK_F2,
		[this]()
		{
			// Photo mode composes its shot through this menu and drives the camera from
			// the same mouse, so handing input back to the game mid-shot fights it for
			// both the pointer and the player's control state. Ignore the shortcut.
			if (PhotoModeSystem.IsActive())
			{
				return;
			}

			bImGuiInteractive = !bImGuiInteractive;
		},
		[this]() { return bImGuiInteractive; } });

	Keybinds.RegisterAction({ "player.godmode", "Toggle God Mode", "Player", VK_F5,
		[this]()
		{
			EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer();
			if (!LocalPlayer)
			{
				return;
			}

			bPlayerGodModeActive = !bPlayerGodModeActive;
			SetPlayerGodMode(*LocalPlayer);
		},
		[this]() { return bPlayerGodModeActive; } });

	Keybinds.RegisterAction({ "player.fly", "Toggle Fly Mode", "Player", VK_F6,
		[]()
		{
			if (!EARS::Modules::Player::GetLocalPlayer())
			{
				return;
			}

			EARS::Modules::PlayerDebugOptions& DebugOptions = *EARS::Modules::PlayerDebugOptions::GetInstance();
			DebugOptions.SetIsInDebugFly(!DebugOptions.IsInDebugFly());
		},
		[]()
		{
			EARS::Modules::PlayerDebugOptions* DebugOptions = EARS::Modules::PlayerDebugOptions::GetInstance();
			return DebugOptions && DebugOptions->IsInDebugFly();
		} });

	Keybinds.RegisterAction({ "world.freeze", "Freeze Game Logic", "World", VK_F7,
		[this]()
		{
			ToggleFreezeLogic();
		},
		[this]() { return bFreezeLogic; } });

	Keybinds.RegisterAction({ "camera.photomode", "Toggle Photo Mode", "Camera", VK_F8,
		[this]() { PhotoModeSystem.Toggle(); },
		[this]() { return PhotoModeSystem.IsActive(); } });

	Keybinds.LoadBindings(Settings::GetCheckedRef().GetKeybindsFilePath());
}










void ImGuiManager::OpenLevelServices()
{
	// apply more events
	LinkMsg(&DefinedEvents::RunningTickEvent, 0x8000);
	LinkMsg(&DefinedEvents::PausedTickEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerAsDriverEnterVehicleEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerAsPassengerEnterVehicleEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerExitVehicleEvent, 0x8000);

	CheckpointDebug.OpenLevelServices();
}

void ImGuiManager::CloseLevelServices()
{
	// reset any existing state applied to player / ui
	bPlayerGodModeActive = false;
	bPlayerVehicleGodModeActive = false;

	// The player this was applied to is going away. Drop the takeover so the next tick
	// with the menu still open re-applies it against the incoming player, rather than
	// the state compare in OnTick seeing no change and skipping it.
	bTakeoverCursor = false;

	// reset things used by mod menu
	TargetFamily = nullptr;
	InventoryAddItem_SelectedName.clear();
	InventoryAddItem_SelectedGuid = {};

	CheckpointDebug.CloseLevelServices();

	PhotoModeSystem.CloseLevelServices();

	UISystem.CloseLevelServices();

	if (bFreezeLogic)
	{
		RWS::MainLoop::Logic::PopPause(16);
		bFreezeLogic = false;
	}

	// remove other events
	UnlinkMsg(&DefinedEvents::RunningTickEvent);
	UnlinkMsg(&DefinedEvents::PausedTickEvent);
	UnlinkMsg(&DefinedEvents::PlayerAsDriverEnterVehicleEvent);
	UnlinkMsg(&DefinedEvents::PlayerAsPassengerEnterVehicleEvent);
	UnlinkMsg(&DefinedEvents::PlayerExitVehicleEvent);

	// Just in case player exits mid-teleport
	UnlinkMsg(&DefinedEvents::iMsgPlayerTeleportDoneExceptFade);

	Mod::ImGuiRuntime::DropFrame();
}

SH::ImGuiCheckpointDebug& ImGuiManager::StaticGetCheckpointDebug()
{
	return ImGuiManager::GetCheckedRef().GetCheckpointDebug();
}

SH::ImGuiUISystem& ImGuiManager::StaticGetUISystemDebug()
{
	return ImGuiManager::GetCheckedRef().GetUISystemDebug();
}


void ImGuiManager::OnTick()
{
	// Poll keyboard shortcuts and fire their actions (menu toggle, god mode, etc.)
	SH::KeybindManager::GetCheckedRef().PollAndDispatch();

	// Update cursor visibility
	// Should only really be present when any ImGui windows are open -
	// The ingame cursor (for menus) is expected to be powered by Apt.
	// Interaction is a separate toggle from visibility: with it off the menu stays
	// on screen but the game keeps the mouse and keyboard.
	// Each transition sends exactly one control event. Player::DisablePlayerControl and
	// EnablePlayerControl are refcounted on the game side (m_PlayerDisableCount), so the
	// pairing is what matters - never re-send on a frame where the state has not moved.
	const bool bCursorVisibilityThisFrame = bShowModMenuWindow && bImGuiInteractive;
	if (bCursorVisibilityThisFrame != bTakeoverCursor)
	{
		bTakeoverCursor = bCursorVisibilityThisFrame;

		EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();

		// Null at the front end and across level loads. The control events are
		// broadcast either way so the state stays consistent for whoever spawns
		// next; only the per-player flag needs a live player.
		EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer();

		if (bTakeoverCursor)
		{
			// DISABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerDisableControlsEventId = hook::Type<RWS::CEventId>(0x112B56C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerDisableControlsEventId, false);

			if (LclPlayer)
			{
				LclPlayer->SetPlayerFlag(EARS::Modules::PlayerFlag::WEAPON_WHEEL_SHOWING);
			}
			//CameraMgr->DisableUpdate();
		}
		else
		{
			// ENABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerEnableControlsEventId = hook::Type<RWS::CEventId>(0x112B39C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerEnableControlsEventId, false);

			if (LclPlayer)
			{
				LclPlayer->ClearPlayerFlag(EARS::Modules::PlayerFlag::WEAPON_WHEEL_SHOWING);
			}

			// The game measures mouse movement as an offset from where it last parked
			// the pointer. Put it back before handing input over, or the first frame of
			// player camera input is a full-screen jump.
			Mod::ImGuiRuntime::RestoreCursorPos();

			//CameraMgr->EnableUpdate();
		}
	}


	Mod::ImGuiRuntime::SetInputOwned(bTakeoverCursor);

	// Builds the frame and calls DrawMenuPanel from inside it.
	Mod::ImGuiRuntime::Tick();
}

void ImGuiManager::DrawMenuPanel()
{
	if (ImGuiManager* const Manager = ImGuiManager::GetChecked())
	{
		Manager->DrawMenu();
	}
}

void ImGuiManager::DrawMenu()
{
#if DEBUG
	if(bShowImGuiStyleEditor)
	{
		ImGui::ShowStyleEditor(&ImGui::GetStyle());
	}
#endif // DEBUG

	if (bShowModMenuWindow)
	{
		// Re-assert the camera-input block every frame while we own input: UIHud's own
		// HideWeaponWheel path clears this flag out from under us if the wheel closes
		// while the menu is up. Keyed on the takeover rather than on the window being
		// visible, or pass-through mode would clear the flag and have it set straight
		// back here on the same tick, leaving the player camera dead.
		if (bTakeoverCursor)
		{
			if (EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer())
			{
				LclPlayer->SetPlayerFlag(EARS::Modules::PlayerFlag::WEAPON_WHEEL_SHOWING);
			}
		}

		if (ImGui::Begin("Scripthook Menu", &bShowModMenuWindow))
		{
			if (ImGui::BeginTabBar("mod_menu_tab_bar"))
			{
				DrawTab_PlayerSettings();

				DrawTab_CheckpointSettings();

				DrawTab_PhotoMode();

				DrawTab_TimeOfDaySettings();

#if SHOW_DEMOGRAPHICS_TAB
				DrawTab_DemographicSettings();
#endif // SHOW_DEMOGRAPHICS_TAB

				DrawTab_ObjectMgrSettings();

#if SHOW_ATTRIBUTEPACKET_WINDOW
				DrawTab_SimMgrSettings();
#endif // SHOW_ATTRIBUTEPACKET_WINDOW

				DrawTab_CitiesSettings();

				DrawTab_BuildingSettings();

#if SHOW_FAMILY_TAB
				DrawTab_FamiliesSettings();
#endif // SHOW_FAMILY_TAB

				DrawTab_PlayerFamilyTreeSettings();

				UISystem.DrawTab();

				DrawTab_Keybinds();

				DrawTab_Support();

				ImGui::EndTabBar();
			}

			ImGui::End();
		}

		CurrentInspector.DrawWindow();
	}
}
