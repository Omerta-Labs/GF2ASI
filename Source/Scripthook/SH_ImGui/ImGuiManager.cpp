#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"
#include "Addons/Settings.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"
#include "Scripthook/SH_ObjectManager/ObjectManager.h"
#include "Scripthook/SH_ImGui/GameEventIds.h"
#include "Scripthook/SH_ImGui/KeybindManager.h"

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


namespace PrivateImGui
{
	void SetupImGuiStyle()
	{
		// Fork of Clean Dark/Red style from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(5.0f, 2.0f);
		style.WindowRounding = 4.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 4.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 4.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(9.0f, 7.0f);
		style.FrameRounding = 4.0f;
		style.FrameBorderSize = 1.0f;
		style.ItemSpacing = ImVec2(9.0f, 8.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 0.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 16.0f;
		style.ScrollbarRounding = 4.0f;
		style.ScrollbarPadding = 4.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.TabBorderSize = 2.0f;
		style.TabBarBorderSize = 2.0f;
		style.TabBarOverlineSize = 1.0f;
		style.TabMinWidthBase = 1.0f;
		style.TabMinWidthShrink = 80;
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		//style.TabMinWidthForCloseButton = 3.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.5f, 0.0f);
		style.ColorMarkerSize = 3.0f;
		style.SelectableTextAlign = ImVec2(0.5f, 0.0f);
		style.SeparatorSize = 1.0f;
		style.SeparatorTextBorderSize = 3.0f;
		style.SeparatorTextAlign = ImVec2(0.0f, 0.5f);

		ImVec4* colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.65f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 0.67f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 0.03f, 1.00f);
		colors[ImGuiCol_CheckboxSelectedBg] = ImVec4(0.22f, 0.48f, 0.80f, 0.45f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.75f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.18f, 0.18f, 0.40f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.67f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_InputTextCursor] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TabSelected] = ImVec4(0.47f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_TabDimmed] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
		colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.40f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.55f, 0.55f, 0.55f, 0.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.36f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.64f, 0.88f, 0.44f);
		colors[ImGuiCol_TreeLines] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.47f, 0.18f, 0.18f, 0.97f);
		colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_UnsavedMarker] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavCursor] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
}

ImGuiManager::ImGuiManager()
	: CEventHandler()
{

}

ImGuiManager::~ImGuiManager()
{
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	ClearDrawDataSnapshot();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX9_Shutdown();
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
	// The EndScene/WndProc hooks may already be live on other threads
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	hook::Type<HWND> windowHandle = hook::Type<HWND>(0x112A024);
	ImGui_ImplWin32_Init(windowHandle);

	hook::Type<IDirect3DDevice9*> Dx9Device = hook::Type<IDirect3DDevice9*>(0x1205750);
	ImGui_ImplDX9_Init(Dx9Device);

	AddFont("scripts/Roboto-Medium.ttf");

	PrivateImGui::SetupImGuiStyle();

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

void ImGuiManager::OnEndScene()
{
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	if (SnapshotDrawData.Valid)
	{
		ImGui_ImplDX9_RenderDrawData(&SnapshotDrawData);
	}
}

void ImGuiManager::OnDeviceLost()
{
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	ClearDrawDataSnapshot();

	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void ImGuiManager::OnDeviceRestored()
{
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void ImGuiManager::CaptureDrawDataSnapshot()
{
	ClearDrawDataSnapshot();

	const ImDrawData* SourceData = ImGui::GetDrawData();
	if (!SourceData || !SourceData->Valid)
	{
		return;
	}

	// Copy the scalar fields (counts, display rect, texture list pointer),
	// then swap the context-owned draw lists for clones we own. The clones
	// stay valid while the context recycles its lists on the next NewFrame.
	SnapshotDrawData = *SourceData;
	SnapshotDrawData.CmdLists.resize(0);
	for (const ImDrawList* SourceList : SourceData->CmdLists)
	{
		SnapshotDrawData.CmdLists.push_back(SourceList->CloneOutput());
	}
}

void ImGuiManager::ClearDrawDataSnapshot()
{
	// ImDrawData::Clear() does not free the lists - it assumes the context
	// owns them, but ours are clones
	for (ImDrawList* ClonedList : SnapshotDrawData.CmdLists)
	{
		IM_DELETE(ClonedList);
	}

	SnapshotDrawData.Clear();
}

void ImGuiManager::AddFont(const char* name)
{
	// ensure we have all fonts existing on disk
	if (!std::filesystem::exists(name))
	{
		//CF_FATAL("Missing font file (%s), cannot run program! Please ensure this file is in the same folder as the tool executable!", FontPath.data());
		return;
	}

	// Add fonts (clear first though)
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();

	//PE: Add all lang.
	static const ImWchar Generic_ranges_everything[] =
	{
	   0x0020, 0xFFFF, // Everything test.
	   0,
	};
	static const ImWchar Generic_ranges_most_needed[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0100, 0x017F,	//0100 — 017F  	Latin Extended-A
		0x0180, 0x024F,	//0180 — 024F  	Latin Extended-B
		0,
	};

	float FONTUPSCALE = 1.0; //Font upscaling.
	float FontSize = 15.0f;

	CustomFont = io.Fonts->AddFontFromFileTTF(name, FontSize * FONTUPSCALE, NULL, &Generic_ranges_everything[0]); //Set as default font.
	if (!CustomFont)
	{
		CustomFont = io.Fonts->AddFontDefault();
	}

	DefaultFont = io.Fonts->AddFontDefault();
}

bool ImGuiManager::HasCursorControl() const
{
	return bTakeoverCursor;
}

void ImGuiManager::NotifySuppressedCursorPos(const int InX, const int InY)
{
	SuppressedCursorPosX = InX;
	SuppressedCursorPosY = InY;
	bHasSuppressedCursorPos = true;
}

void ImGuiManager::RestoreGameCursorPos()
{
	if (!bHasSuppressedCursorPos)
	{
		return;
	}

	// Safe to go straight at the API: the detour only swallows the call while
	// bTakeoverCursor is set, and this runs after it has been cleared.
	::SetCursorPos(SuppressedCursorPosX, SuppressedCursorPosY);
	bHasSuppressedCursorPos = false;
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

	// Ticks have stopped but EndScene keeps firing; drop the last frame so it
	// doesn't linger over the loading screen
	{
		std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);
		ClearDrawDataSnapshot();
	}
}

SH::ImGuiCheckpointDebug& ImGuiManager::StaticGetCheckpointDebug()
{
	return ImGuiManager::GetCheckedRef().GetCheckpointDebug();
}

SH::ImGuiUISystem& ImGuiManager::StaticGetUISystemDebug()
{
	return ImGuiManager::GetCheckedRef().GetUISystemDebug();
}

LRESULT ImGuiManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// The handler appends to the context's shared input queue, which must not
	// overlap the SIM thread's frame build
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
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
			RestoreGameCursorPos();

			//CameraMgr->EnableUpdate();
		}
	}

	// Everything below touches the live ImGui context; keep the presentation
	// and window threads out until this frame's draw data is snapshotted
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	ImGuiIO& IO = ImGui::GetIO();
	IO.MouseDrawCursor = bTakeoverCursor;

	// The window hook feeds every input message to the backend and the Win32 handler
	// swallows none of them, so ImGui would still hover and click against a cursor the
	// game is busy recentring. Mask its input off while it does not own the mouse.
	if (bTakeoverCursor)
	{
		IO.ConfigFlags &= ~(ImGuiConfigFlags_NoMouse | ImGuiConfigFlags_NoKeyboard);
	}
	else
	{
		IO.ConfigFlags |= (ImGuiConfigFlags_NoMouse | ImGuiConfigFlags_NoKeyboard);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	
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

	ImGui::EndFrame();
	ImGui::Render();

	// Publish a complete frame for the presentation thread to consume
	CaptureDrawDataSnapshot();
}
