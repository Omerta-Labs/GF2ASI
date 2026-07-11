#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"
#include "Addons/Settings.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"
#include "Scripthook/SH_ObjectManager/ObjectManager.h"
#include "Scripthook/SH_ImGui/KeybindManager.h"

// Godfather
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Core/SimManager/SimManager.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"
#include "SDK/EARS_Framework/MainLoop/Logic.h"
#include "SDK/EARS_Framework/Toolkits/GroupManager/GroupManager.h"
#include "SDK/EARS_Godfather/Modules/Buildings/Building.h"
#include "SDK/EARS_Godfather/Modules/Buildings/BuildingStore.h"
#include "SDK/EARS_Godfather/Modules/Buildings/BuildingManager.h"
#include "SDK/EARS_Godfather/Modules/Families/Family.h"
#include "SDK/EARS_Godfather/Modules/Families/FamilyManager.h"
#include "SDK/EARS_Godfather/Modules/Families/CorleoneData.h"
#include "SDK/EARS_Godfather/Modules/Families/MadeMan.h"
#include "SDK/EARS_Godfather/Modules/Item/InventoryManager.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDebug.h"
#include "SDK/EARS_Godfather/Modules/Mobface/MobfaceManager.h"
#include "SDK/EARS_Godfather/Modules/TimeOfDay/TimeOfDayManager.h"
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/NPC/Crime/CrimeManager.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegion.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegionManager.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/Behaviours/WhiteboxCar/WhiteboxCar.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/VehicleDamageComponent.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"
#include "SDK/EARS_Physics/Vehicles/ground/wheeled/HavokWheeledVehicle.h"

#include "SDK/EARS_RT_LLRender/include/ShaderManager.h"

// CPP
#include <filesystem>
#include <string>

#define ENABLE_ENTITY_SPAWN_DEBUG 0

#define SHOW_ATTRIBUTEPACKET_WINDOW 0

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

namespace DefinedEvents
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	static hook::Type<RWS::CEventId> PausedTickEvent = hook::Type<RWS::CEventId>(0x12069B4);
	static hook::Type<RWS::CEventId> DoRenderEvent = hook::Type<RWS::CEventId>(0x01206970);
	static hook::Type<RWS::CEventId> PreRenderEvent = hook::Type<RWS::CEventId>(0x01206980);
	static hook::Type<RWS::CEventId> PlayerAsDriverEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E030);
	static hook::Type<RWS::CEventId> PlayerAsPassengerEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E11C);
	static hook::Type<RWS::CEventId> PlayerExitVehicleEvent = hook::Type<RWS::CEventId>(0x112E018);

	static hook::Type<RWS::CEventId> iMsgStreamLoadComplete = hook::Type<RWS::CEventId>(0x1206760);
	static hook::Type<RWS::CEventId> iMsgStreamUnloadComplete = hook::Type<RWS::CEventId>(0x1206768);
	static hook::Type<RWS::CEventId> iMsgStreamBeginUnload = hook::Type<RWS::CEventId>(0x1206778);
	static hook::Type<RWS::CEventId> iMsgStreamCancel = hook::Type<RWS::CEventId>(0x1206780);
	static hook::Type<RWS::CEventId> iMsgStreamAllDispatched = hook::Type<RWS::CEventId>(0x120678C);
	static hook::Type<RWS::CEventId> iMsgStreamBeginLoad = hook::Type<RWS::CEventId>(0x1206794);
	static hook::Type<RWS::CEventId> iMsgStreamIdle = hook::Type<RWS::CEventId>(0x12067A4);
	static hook::Type<RWS::CEventId> iMsgStreamUnloading = hook::Type<RWS::CEventId>(0x12067B4);

	static hook::Type<RWS::CEventId> iMsgPlayerTeleportDoneExceptFade = hook::Type<RWS::CEventId>(0x112B344);
}

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
		[this]() { bImGuiInteractive = !bImGuiInteractive; },
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

	Keybinds.LoadBindings(Settings::GetCheckedRef().GetConfigFilePath());
}

void ImGuiManager::SetPlayerGodMode(EARS::Modules::Player& InPlayer) const
{
	EARS::Modules::StandardDamageComponent* DamageComp = InPlayer.GetDamageComponent();
	DamageComp->SetInvincible(bPlayerGodModeActive);
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

void ImGuiManager::DrawTab_PlayerSettings()
{
	if (ImGui::BeginTabItem("Player", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
		{
			if (ImGui::CollapsingHeader("Players State", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Toggle settings such as NoClip and GodMode");

				ImGui::Text("Current Controller ID: %u", LocalPlayer->GetControllerID());

				EARS::Modules::PlayerDebugOptions& DebugOptions = *EARS::Modules::PlayerDebugOptions::GetInstance();

				bool bIsFlyActive = DebugOptions.IsInDebugFly();
				if (ImGui::Checkbox("Fly Mode", &bIsFlyActive))
				{
					DebugOptions.SetIsInDebugFly(bIsFlyActive);
				}

				bool bNewGodModeActive = bPlayerGodModeActive;
				if (ImGui::Checkbox("God Mode", &bNewGodModeActive))
				{
					bPlayerGodModeActive = bNewGodModeActive;
					SetPlayerGodMode(*LocalPlayer);
				}

				bool bNewFreezeGameLogic = bFreezeLogic;
				if (ImGui::Checkbox("Freeze Game Logic", &bNewFreezeGameLogic))
				{
					ToggleFreezeLogic();
				}

				if (ImGui::Button("Inspect Player"))
				{
					InitialiseNPCInspector(LocalPlayer, true);
				}

				if (EARS::Modules::CrimeManager* CrimeMgr = EARS::Modules::CrimeManager::GetInstance())
				{
					if (ImGui::Button("Call off the police"))
					{
						CrimeMgr->CalmPoliceTowardsCorleones();
					}
				}
			}

			if (ImGui::CollapsingHeader("Players Inventory", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Modify Players Inventory (Unlimited Ammo, giving weapons has been moved to ObjectManager)");

				Mod::ObjectManager& ObjMgrRef = Mod::ObjectManager::GetCheckedRef();

				if (EARS::Modules::InventoryManager* PlayerInventoryMgr = LocalPlayer->GetInventoryManager())
				{
					const char* Label = PlayerInventoryMgr->HasPlayerInfiniteAmmo() ? "Remove Unlimited Ammo" : "Give Unlimited Ammo";
					if (ImGui::Button(Label))
					{
						PlayerInventoryMgr->ToggleUnlimitedAmmo();
					}
				}
			}

			if (ImGui::CollapsingHeader("Players Family", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Modify any characteristics of the Family the Player is part of");

				if (EARS::Modules::Family* PlayersFamily = LocalPlayer->GetFamily())
				{
					static float DesiredMoney = 0.0f;

					// button
					if (ImGui::Button("Modify Balance"))
					{
						PlayersFamily->ModifyBalance(DesiredMoney, EARS::Modules::LedgerItemType::LEDGERITEMTYPE_REVENUE_OTHER);
					}

					ImGui::SameLine();

					// entry box
					ImGui::PushItemWidth(-1.0f);
					ImGui::InputFloat("###modify_balance", &DesiredMoney);
					ImGui::PopItemWidth();
				}
			}
		
			if (ImGui::CollapsingHeader("Players Vehicle", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Modify any characteristics of the Vehicle the Player occupies");

				if (EARS::Vehicles::WhiteboxCar* CurrentCar = LocalPlayer->GetVehicle())
				{
					ImGui::Text("Current Car: 0x%X", CurrentCar);

					bool bNewVehicleGodModeActive = bPlayerVehicleGodModeActive;
					if (ImGui::Checkbox("Vehicle God Mode", &bNewVehicleGodModeActive))
					{
						SetVehicleGodMode(CurrentCar, bNewVehicleGodModeActive);
						bPlayerVehicleGodModeActive = bNewVehicleGodModeActive;
					}
				}
				else
				{
					ImGui::TextColored({ 255, 0, 0, 255 }, "Player is not in a car, cannot show options");
				}
			}
		}
		else
		{
			ImGui::Text("Local Player is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_CheckpointSettings()
{
	if (ImGui::BeginTabItem("Checkpoints", nullptr, ImGuiTabItemFlags_None))
	{
		GetCheckpointDebug().DisplayTab();
		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_PhotoMode()
{
	if (ImGui::BeginTabItem("Photo Mode", nullptr, ImGuiTabItemFlags_None))
	{
		PhotoModeSystem.DrawTab();
		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_TimeOfDaySettings()
{
	if (ImGui::BeginTabItem("Time Of Day", nullptr, ImGuiTabItemFlags_None))
	{
		EARS::Modules::TimeOfDayManager* TODManager = EARS::Modules::TimeOfDayManager::GetInstance();
		if (TODManager)
		{
			EARS::Modules::TimeOfDayManager::GameTime CurrentTime = TODManager->GetGameTime();

			ImGui::TextDisabled("Year/Day/Hour/Minute");
			if (ImGui::InputInt4("##time_of_day_input", &CurrentTime.m_Year, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				TODManager->SetGameTime(CurrentTime);
			}
		}
		else
		{
			ImGui::Text("Time Of Day is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_DemographicSettings()
{
#if SHOW_DEMOGRAPHICS_TAB
	if (ImGui::BeginTabItem("Demographic Regions", nullptr, ImGuiTabItemFlags_None))
	{
		EARS::Modules::DemographicRegionManager* DRMgr = EARS::Modules::DemographicRegionManager::GetInstance();
		if (DRMgr)
		{
			ImGui::Text("Current Region: %p", DRMgr->GetCurrentRegion());

			if (ImGui::TreeNode("Registered Regions"))
			{
				DRMgr->ForEachDemographicRegion([](const EARS::Modules::DemographicRegion& InRegion) {
						const std::string RegionName = InRegion.GetDebugName();
						ImGui::Text("%s - (%p)", RegionName.data(), &InRegion);
					});

				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("Demographic Regions Manager is missing!");
		}

		ImGui::EndTabItem();
	}
#endif // SHOW_DEMOGRAPHICS_TAB
}

void ImGuiManager::DrawTab_CitiesSettings()
{
	if (ImGui::BeginTabItem("Cities", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Modules::CityManager* CityMgr = EARS::Modules::CityManager::GetInstance())
		{
			const uint32_t CurrentCityID = CityMgr->GetCurrentCity();
			const String* CurrentCityName = CityMgr->GetDisplayName(CurrentCityID);
			ImGui::Text("Current City: %s", (CurrentCityName ? CurrentCityName->c_str() : "None"));

			if (ImGui::TreeNode("Registered Cities"))
			{
				CityMgr->ForEachCity([](EARS::Modules::City& InCity) {
					if (ImGui::TreeNodeEx((void*)InCity.GetCityID(), ImGuiTreeNodeFlags_DefaultOpen, "%s", InCity.GetDisplayName()->c_str()))
					{
						bool bIsVisible = InCity.IsKnownToPlayer();
						if (ImGui::Checkbox("Is Visible To Player", &bIsVisible))
						{
							if (bIsVisible)
							{
								// Switch to visible
								InCity.RevealToPlayer();
							}
							else
							{
								// switch to hidden
								InCity.HideFromPlayer();
							}
						}

						if (ImGui::Button("Travel To City"))
						{
							InCity.RequestTeleport();
						}

						ImGui::TreePop();
					}
					});

				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("City Manager is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_BuildingSettings()
{
	EARS::Modules::FamilyManager* FamilyMgr = EARS::Modules::FamilyManager::GetInstance();
	EARS::Modules::BuildingManager* BuildingMgr = EARS::Modules::BuildingManager::GetInstance();
	EARS::Modules::CityManager* CityMgr = EARS::Modules::CityManager::GetInstance();
	if (!BuildingMgr || !FamilyMgr || !CityMgr)
	{
		return;
	}

	// Utility which implements Family takeover for a single building
	auto DrawFamilyComboBox = [&](EARS::Modules::BuildingStore& Store)
		{
			const EARS::Modules::Family* OwningFamily = FamilyMgr->GetFamily(Store.GetFamilyID());
			const String* FamilyString = OwningFamily->GetInternalName();

			ImGui::PushItemWidth(-1.0f);
			if (ImGui::BeginCombo("###family_selector", FamilyString->c_str()))
			{
				FamilyMgr->ForEachStrategyFamily([&](const EARS::Modules::Family& StrategyFamily)
					{
						const String* SelectableName = StrategyFamily.GetInternalName();
						if (ImGui::Selectable(SelectableName->c_str(), StrategyFamily.GetFamilyID() == Store.GetFamilyID()))
						{
							Store.ChangeOwnership(StrategyFamily.GetFamilyID(), false, nullptr, false);
						}
					});

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		};

	auto DrawPlayerTeleportButton = [&](EARS::Modules::BuildingStore& Store)
		{
			ImGui::PushItemWidth(-1.0f);
			if (ImGui::Button("Teleport"))
			{
				DeferredTeleportPayload = { .TeleportLocation = Store.GetEntrancePos() };

				if (CityMgr->GetCurrentCity() != Store.GetCityID())
				{
					// we need to first teleport to new city to get world partitions sync'd up
					CityMgr->TeleportToCity(Store.GetCityID());

					LinkMsg(&DefinedEvents::iMsgPlayerTeleportDoneExceptFade, 0x8000);
				}
				else
				{
					ProcessBuildingTeleport();
				}
			}
			ImGui::PopItemWidth();
		};

	if (ImGui::BeginTabItem("Buildings", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::BeginChild("building_store_table");
		if (ImGui::BeginTable("active_building_table", 5, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
		{
			const float AvailWidth = ImGui::GetContentRegionAvail().x;
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.08f);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.42f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.1f);
			ImGui::TableSetupColumn("Family", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.2f);
			ImGui::TableSetupColumn("Teleport", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.2f);
			ImGui::TableHeadersRow();

			BuildingMgr->ForEachBuildingStore([&](EARS::Modules::BuildingStore& ActiveStore)
				{
					ImGui::PushID(&ActiveStore);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					const String* BuildingString = ActiveStore.GetDisplayName();

					const EARS::Modules::Family* OwningFamily = FamilyMgr->GetFamily(ActiveStore.GetFamilyID());
					const String* FamilyString = OwningFamily->GetInternalName();

					ImGui::Text("%u", ActiveStore.GetVenueID());
					ImGui::TableNextColumn();
					ImGui::Text("%s", BuildingString->c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", BuildingMgr->GetBuildingTypeInternalName(ActiveStore.GetBuildingType()));
					ImGui::TableNextColumn();
					DrawFamilyComboBox(ActiveStore);
					ImGui::TableNextColumn();
					DrawPlayerTeleportButton(ActiveStore);
					ImGui::TableNextColumn();

					ImGui::PopID();
				});


			ImGui::EndTable();
		}
		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_FamiliesSettings()
{
#if SHOW_FAMILY_TAB
	EARS::Modules::FamilyManager* FamilyMgr = EARS::Modules::FamilyManager::GetInstance();
	if (!FamilyMgr)
	{
		return;
	}

	if (ImGui::BeginTabItem("Families", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::BeginChild("family_contents");

		const char* Preview = "<select_family>";
		if (TargetFamily)
		{
			Preview = TargetFamily->GetInternalName()->c_str();
		}

		ImGui::PushItemWidth(-1.0f);
		if(ImGui::BeginCombo("###select_family", Preview))
		{
			FamilyMgr->ForEachStrategyFamily([&](EARS::Modules::Family& InFamily) 
			{
				const char* FamilyName = InFamily.GetInternalName()->c_str();
				bool bSelected = (TargetFamily == &InFamily);
				if(ImGui::Selectable(FamilyName, &bSelected))
				{
					TargetFamily = &InFamily;
				}
			});

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		if (TargetFamily)
		{
			if (ImGui::CollapsingHeader("Strategy Game", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Compound Venue ID: %u", TargetFamily->GetCompoundVenueID());

				ImGui::BeginDisabled(!TargetFamily->HasBeenEliminated());
				if (ImGui::Button("Revive Family"))
				{
					TargetFamily->ReviveFamily();
				}
				ImGui::EndDisabled();

				float MinTurnInterval = TargetFamily->GetMinTurnInterval();
				if (ImGui::InputFloat("Min Turn Interval", &MinTurnInterval))
				{
					TargetFamily->SetMinTurnInterval(MinTurnInterval);
				}

				float MaxTurnInterval = TargetFamily->GetMaxTurnInterval();
				if (ImGui::InputFloat("Max Turn Interval", &MaxTurnInterval))
				{
					TargetFamily->SetMaxTurnInterval(MaxTurnInterval);
				}

				float ResponseDelay = TargetFamily->GetResponseDelay();
				if (ImGui::InputFloat("Response Delay", &ResponseDelay))
				{
					TargetFamily->SetResponseDelay(ResponseDelay);
				}

				/*const uint32_t FamilyAlly1 = TargetFamily->GetAllyFamilyID(1);
				ImGui::Text("Ally 1: %u", FamilyAlly1);
				const uint32_t FamilyAlly2 = TargetFamily->GetAllyFamilyID(2);
				ImGui::Text("Ally 2: %u", FamilyAlly2);
				const uint32_t FamilyAlly3 = TargetFamily->GetAllyFamilyID(3);
				ImGui::Text("Ally 3: %u", FamilyAlly3);*/
			}

			if (ImGui::CollapsingHeader("Made Men", ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (uint32_t i = 0; i < TargetFamily->GetNumMadeMen(); i++)
				{
					EARS::Modules::MadeMan* CurMadeMan = TargetFamily->GetMadeManByIndex(i);
					const String* Name = CurMadeMan->GetSimNPC()->GetName();
					if (ImGui::TreeNode(CurMadeMan, "%s", Name->c_str()))
					{
						ImGui::BulletText("State: %s", EARS::Modules::MadeMan::StateEnumToString(CurMadeMan->GetState()));
						ImGui::BulletText("Venue ID: %u", CurMadeMan->GetVenueID());
						ImGui::BulletText("Rank: %u", CurMadeMan->GetRank());
						ImGui::BulletText("State Cooldown: %f", CurMadeMan->GetCountdown());

						switch (CurMadeMan->GetState())
						{
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_HOSPITAL:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_JAIL:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_COOLDOWN:
							{
								if (ImGui::Button("Send To Component"))
								{
									CurMadeMan->SendToCompound();
								}
								ImGui::SameLine();
								if (ImGui::Button("Eliminate"))
								{
									TargetFamily->KillMadeMan(*CurMadeMan->GetSimNPC());
								}
								break;
							}
							case EARS::Modules::MadeManState::MADE_MAN_STATE_ELIMINATED:
							{
								if (ImGui::Button("Revive Made Man"))
								{
									TargetFamily->ReviveMadeMan(*CurMadeMan->GetSimNPC());
								}
								break;
							}
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_COMBAT:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_HIDDEN:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_TRANSIT:
							{
								// TODO: Leave as is?
								break;
							}
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IDLE:
							{
								if (ImGui::Button("Hospitalize"))
								{
									TargetFamily->HospitalizeMadeMan(*CurMadeMan->GetSimNPC());
								}
								ImGui::SameLine();
								if (ImGui::Button("Incarcerate"))
								{
									TargetFamily->IncarcerateMadeMan(*CurMadeMan->GetSimNPC());
								}
								ImGui::SameLine();
								if (ImGui::Button("Eliminate"))
								{
									TargetFamily->KillMadeMan(*CurMadeMan->GetSimNPC());
								}
								break;
							}
						}

						ImGui::TreePop();

					}
				}
			}

			if (ImGui::CollapsingHeader("Omerta Table", ImGuiTreeNodeFlags_DefaultOpen))
			{
				TargetFamily->ForEachOmertaTable([&](EARS::Modules::Family::OmertaEntry& OmertaEntry)
					{
						const EARS::Modules::Family* TargetFamily = FamilyMgr->GetFamily(OmertaEntry.m_FamilyID);
						const char* FamilyName = TargetFamily->GetInternalName()->c_str();

						ImGui::InputFloat(FamilyName, &OmertaEntry.m_Omerta);
					});
			}
		}

		ImGui::EndChild();

		ImGui::EndTabItem();
	}
#endif // SHOW_FAMILY_TAB
}

void ImGuiManager::DrawTab_PlayerFamilyTreeSettings()
{
	Mod::ObjectManager& ObjMgr = Mod::ObjectManager::GetCheckedRef();

	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();

	if (ImGui::BeginTabItem("Player Family Tree Settings", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::BeginChild("family_tree_settings_window");

		EARS::Modules::CorleoneFamilyData* FamilyData = EARS::Modules::CorleoneFamilyData::GetInstance();
		if (!FamilyData)
		{
			ImGui::Text("ERROR: Missing CorleoneFamilyData instance");
		}

		EARS::Modules::PlayerFamilyTree* FamilyTreeData = EARS::Modules::PlayerFamilyTree::GetInstance();
		if (!FamilyTreeData)
		{
			ImGui::Text("ERROR: Missing PlayerFamilyTree instance");
		}

		if (!FamilyData || !FamilyTreeData)
		{
			ImGui::EndTabItem();
			return;
		}


		// let the user toggle pre-order bonuses
		bool bCurrentPreOrderFlag = FamilyData->HasUnlockedPreOrderCrew();
		if (ImGui::Checkbox("Unlock Pre-Order Crew", &bCurrentPreOrderFlag))
		{
			if (bCurrentPreOrderFlag)
			{
				FamilyData->UnlockPreOrderCrew();
			}
			else
			{
				FamilyData->LockPreOrderCrew();
			}
		}

		if (ImGui::CollapsingHeader("Crew Members (simple)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Add all members to crew"))
			{
				FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {
					if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
					{
						if (MadeManNPC->GetIsCrewMember() == false)
						{
							InMember.JoinCrew();
						}
					}
					});
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove all members from crew"))
			{
				FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {
					if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
					{
						if (MadeManNPC->GetIsCrewMember() == true)
						{
							InMember.LeaveCrew();
						}
					}
					});
			}

			ImGui::SameLine();

			if (ImGui::Button("Unlock Full Tree"))
			{
				FamilyTreeData->SetCurrentTreeType(EARS::Modules::PlayerFamilyTree::FamilyTreeType::FAMILYTREE_TYPE_CONSIGLIORE_UNDERBOSS_2CAPOS_4SOLDIERS);
			}
		}

		if (ImGui::CollapsingHeader("Crew Members (detailed)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			uint32_t CurrentIdx = 0;
			FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {

				const char* Name = "[UNKNOWN]";
				if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
				{
					String* NPC_Name = MadeManNPC->GetName();
					Name = NPC_Name->c_str();
				}

				if (ImGui::TreeNode(&InMember, "Member[%u] -> '%s'", CurrentIdx, Name))
				{
					ImGui::Text("SimNPC: %p", InMember.GetSimNPC());
					ImGui::Text("Flags: %u", InMember.GetFlags().GetAllFlags());
					ImGui::Text("Rank: %i", InMember.GetRank());

					const EARS::Common::guid128_t WeaponGUID = InMember.GetWeaponGUID();
					ImGui::Text("Weapon GUID: [%p %p %p %p]", WeaponGUID.a, WeaponGUID.b, WeaponGUID.c, WeaponGUID.d);

					if (EARS::Modules::SimNPC* SimulatedNPC = InMember.GetSimNPC())
					{
						if (ImGui::Button("Toggle Spawn (As Crew Member)"))
						{
							const bool bInCrew = SimulatedNPC->GetIsCrewMember();
							if (SimulatedNPC->GetIsCrewMember())
							{
								InMember.LeaveCrew();
							}
							else
							{
								InMember.JoinCrew();
							}
						}

						if (EARS::Modules::NPC* CrewNPC = SimulatedNPC->GetNPC())
						{
							if (ImGui::Button("Inspect"))
							{
								InitialiseNPCInspector(CrewNPC, false);
							}
						}

						// Provide the option to change weapon license for this character
						const EARS::Common::guid128_t SimNPCID = SimulatedNPC->InqInstanceID();
						uint8_t WeaponLicense = FamilyData->GetWeaponLicense(SimNPCID);
						if (ImGui::SliderScalar("Weapon License", ImGuiDataType_U8, &WeaponLicense, &EARS::Modules::CorleoneFamilyData::MIN_WEAPON_LICENSE, &EARS::Modules::CorleoneFamilyData::MAX_WEAPON_LICENSE))
						{
							FamilyData->SetWeaponLicense(SimNPCID, WeaponLicense);
						}
					}

					if (ImGui::TreeNode("Specialties"))
					{
						auto RenderCheckBox = [&InMember](const std::string& Name, const EARS::Modules::Specialties Index)
							{
								bool bValue = InMember.HasSpecialty(Index);
								if (ImGui::Checkbox(Name.data(), &bValue))
								{
									InMember.ToggleSpecialty(Index);
								}
							};

						RenderCheckBox("Demolitions", EARS::Modules::Specialties::SPECIALITY_DEMO);
						RenderCheckBox("Arsonist", EARS::Modules::Specialties::SPECIALITY_ARSONIST);
						RenderCheckBox("Safecracker", EARS::Modules::Specialties::SPECIALITY_SAFECRACKER);
						RenderCheckBox("Engineer", EARS::Modules::Specialties::SPECIALITY_ENGINEER);
						RenderCheckBox("Medic", EARS::Modules::Specialties::SPECIALITY_MEDIC);
						RenderCheckBox("Bruiser", EARS::Modules::Specialties::SPECIALITY_BRUISER);

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Replace Made Man"))
					{
						Mod::ObjectEntryList& SimNPCList = ObjMgr.GetSimNPCList();
						SimNPCList.DrawList();

						if (ImGui::Button("Replace"))
						{
							const EARS::Common::guid128_t TargetGUID = SimNPCList.GetSelectedGUID();
							if (RWS::CAttributePacket* FoundPacket = SimMgr->GetAttributePacket(&TargetGUID, 0))
							{
								auto PacketIt = FoundPacket->GetEntityIterator();
								RWS::CAttributeHandler* FirstHandler = PacketIt.GetEntity_Mutable();
								EARS::Modules::SimNPC* AsSimNPC = static_cast<EARS::Modules::SimNPC*>(FirstHandler);

								// need to remove prior family member
								auto FoundSlotIndex = FamilyTreeData->FindTreeSlotIndex(InMember.GetSimNPC());
								if (FoundSlotIndex != EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_INVALID)
								{
									FamilyTreeData->RemoveFamilyMember(FoundSlotIndex, true);
								}

								// by default rely on existing member
								// TODO: Fetch from the SimNPC
								uint32_t DesiredSpecialities = InMember.GetSpecialities();
								if (DesiredSpecialities == 0)
								{
									DesiredSpecialities = (uint32_t)EARS::Modules::Specialties::SPECIALITY_ARSONIST;
								}

								// We must release MobFace, otherwise we may risk a crash with non-mobface types.
								EARS::Modules::MobfaceManager& MobFaceMgr = *EARS::Modules::MobfaceManager::GetInstance();
								MobFaceMgr.ResetMobfaceForSlot(FoundSlotIndex);

								// now replace with the new family member
								FamilyTreeData->AddFamilyMember(
									InMember.GetRank(),
									AsSimNPC,
									DesiredSpecialities,
									EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_INVALID,
									nullptr /* weapon guid */);
							}
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}

				CurrentIdx++;

				});
		}

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_ObjectMgrSettings()
{
	if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
	{
		if (ImGui::BeginTabItem("Object Manager"))
		{
			Mod::ObjectManager& ObjMgrRef = Mod::ObjectManager::GetCheckedRef();
			ObjMgrRef.ImGuiDrawContents();

			ImGui::EndTabItem();
		}
	}
}

void ImGuiManager::DrawTab_SimMgrSettings()
{
	if (ImGui::BeginTabItem("Sim Manager"))
	{
		EARS::Framework::SimManager& SimMgr = *EARS::Framework::SimManager::GetInstance();
		EARS::Framework::StreamManager& StreamMgr = *EARS::Framework::StreamManager::GetInstance();

		static RWS::CAttributePacket* FoundPacket = nullptr;

		static EARS::Common::guid128_t PacketGUID;

		ImGui::Text("Search for a Packet:");
		ImGui::InputScalarN("###packet_search", ImGuiDataType_U32, &PacketGUID.a, 4);
		if (ImGui::Button("Find"))
		{
			FoundPacket = SimMgr.GetAttributePacket(&PacketGUID, 0);
		}

		if (FoundPacket)
		{
			const EARS::Common::guid128_t PacketID = FoundPacket->GetInstanceID();
			const uint32_t ClassID = FoundPacket->GetIdOfClassToCreate();
			const uint32_t StreamHdl = FoundPacket->GetStreamHandle();

			EARS::Framework::Stream* Str = StreamMgr.GetStreamFromHandle(StreamHdl);
			const char* StrFilename = Str->GetFileName();

			ImGui::Text("Parent Stream: %s (%u)", StrFilename, StreamHdl);
			ImGui::Value("ClassID", ClassID);
			
			auto EntityIt = FoundPacket->GetEntityIterator();
			while (EntityIt.IsFinished() == false)
			{
				ImGui::Text("%p", EntityIt.GetEntity());

				EARS::Framework::Entity* AsEntity = reinterpret_cast<EARS::Framework::Entity*>(EntityIt.GetEntity_Mutable());

				EntityIt++;
			}

		}

		ImGui::BeginChild("simgroup_test");
		SimMgr.ForEachPacket([&](const RWS::CAttributePacket& Pckt)
			{
				const EARS::Common::guid128_t ID = Pckt.GetInstanceID();
				ImGui::Text("[%u] %u - %u - %u - %u", ID.a, ID.b, ID.c, ID.d, Pckt.GetStreamHandle());
			});
		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_Keybinds()
{
	if (!ImGui::BeginTabItem("Keybinds", nullptr, ImGuiTabItemFlags_None))
	{
		return;
	}

	SH::KeybindManager& Keybinds = SH::KeybindManager::GetCheckedRef();

	ImGui::TextWrapped("Bind keyboard shortcuts to menu actions. Click Rebind then press a key; "
		"values are stored as Windows virtual-key codes in gf2asi.ini.");

	if (Keybinds.IsRebinding())
	{
		ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Press a key to bind, or Escape to cancel...");
	}
	else
	{
		// Keep the layout stable whether or not a capture is in progress
		ImGui::TextDisabled("Ready.");
	}

	const ImGuiTableFlags TableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp;
	if (ImGui::BeginTable("KeybindTable", 4, TableFlags))
	{
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("##Buttons", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (const SH::ShortcutAction& Action : Keybinds.GetActions())
		{
			ImGui::TableNextRow();
			ImGui::PushID(Action.Id.c_str());

			// Action name, with the category and id available on hover
			ImGui::TableNextColumn();
			ImGui::TextUnformatted(Action.DisplayName.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s  (%s)", Action.Id.c_str(), Action.Category.c_str());
			}

			// Live toggle state, for actions that report it
			ImGui::TableNextColumn();
			if (Action.IsActive)
			{
				const bool bActive = Action.IsActive();
				ImGui::TextColored(bActive ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f) : ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
					bActive ? "ON" : "off");
			}
			else
			{
				ImGui::TextDisabled("-");
			}

			// Current binding
			ImGui::TableNextColumn();
			const bool bCapturingThis = Keybinds.IsRebinding() && Keybinds.GetRebindTargetId() == Action.Id;
			const int VirtualKey = Keybinds.GetBinding(Action.Id);
			if (bCapturingThis)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "[ ... ]");
			}
			else if (VirtualKey == 0)
			{
				ImGui::TextDisabled("Unbound");
			}
			else
			{
				const std::string KeyName = SH::KeybindManager::GetKeyDisplayName(VirtualKey);
				ImGui::TextUnformatted(KeyName.c_str());
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Virtual-key 0x%02X", VirtualKey);
				}
			}

			// Rebind / clear controls, locked out while another capture is running
			ImGui::TableNextColumn();
			ImGui::BeginDisabled(Keybinds.IsRebinding());
			if (ImGui::SmallButton("Rebind"))
			{
				Keybinds.BeginRebind(Action.Id);
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("Clear"))
			{
				Keybinds.SetBinding(Action.Id, 0);
			}
			ImGui::EndDisabled();

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	ImGui::EndTabItem();
}

void ImGuiManager::DrawTab_Support()
{
	auto AddUnderLine = [](ImColor col_)
		{
			ImVec2 min = ImGui::GetItemRectMin();
			ImVec2 max = ImGui::GetItemRectMax();
			min.y = max.y;
			ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
		};

	auto TextURL = [&AddUnderLine](const char* Name, const char* URL)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			ImGui::Text(Name);
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseClicked(0))
				{
					ShellExecuteA(0, 0, URL, 0, 0, SW_SHOW);
				}

				AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
				ImGui::SetTooltip("Open in browser\n\t %s", URL);
			}
			else
			{
				AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
			}
		};

	// shamelessly plug donations
	if (ImGui::BeginTabItem("Support", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::TextWrapped("If you want to support the development of this project, please consider donating! Donations with accompanying feature requests will be considered for upcoming versions.");
		TextURL("Patreon", "https://www.patreon.com/Greavesy");
		TextURL("Ko-fi", "https://ko-fi.com/greavesy");
		TextURL("Boosty", "https://boosty.to/greavesy/donate");

#if DEBUG
		if(ImGui::Button("Show ImGui Style Editor"))
		{
			bShowImGuiStyleEditor = true;
		}
#endif // DEBUG

		ImGui::EndTabItem();
	}
}

void ImGuiManager::OnTick()
{
	// Poll keyboard shortcuts and fire their actions (menu toggle, god mode, etc.)
	SH::KeybindManager::GetCheckedRef().PollAndDispatch();

	// Update cursor visibility
	// Should only really be present when any ImGui windows are open - 
	// The ingame cursor (for menus) is expected to be powered by Apt.
	const bool bCursorVisibilityThisFrame = bShowModMenuWindow;
	if (bCursorVisibilityThisFrame != bTakeoverCursor)
	{
		bTakeoverCursor = bCursorVisibilityThisFrame;

		EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();

		if (bTakeoverCursor)
		{
			// DISABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerDisableControlsEventId = hook::Type<RWS::CEventId>(0x112B56C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerDisableControlsEventId, false);

			EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer();
			LclPlayer->SetPlayerFlag(EARS::Modules::PlayerFlag::WEAPON_WHEEL_SHOWING);
			//CameraMgr->DisableUpdate();
		}
		else
		{
			// ENABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerEnableControlsEventId = hook::Type<RWS::CEventId>(0x112B39C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerEnableControlsEventId, false);

			EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer();
			LclPlayer->ClearPlayerFlag(EARS::Modules::PlayerFlag::WEAPON_WHEEL_SHOWING);

			//CameraMgr->EnableUpdate();
		}
	}

	// Everything below touches the live ImGui context; keep the presentation
	// and window threads out until this frame's draw data is snapshotted
	std::lock_guard<std::recursive_mutex> ContextLock(ImGuiContextLock);

	ImGuiIO& IO = ImGui::GetIO();
	IO.MouseDrawCursor = bShowModMenuWindow;

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
		// force every frame when the UI is active
		EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer();
		LclPlayer->SetPlayerFlag(EARS::Modules::PlayerFlag::WEAPON_WHEEL_SHOWING);

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

bool ImGuiManager::SetVehicleGodMode(EARS::Vehicles::WhiteboxCar* InVehicle, bool bGodModeActive) const
{
	if (InVehicle)
	{
		EARS::Modules::StandardDamageComponent* DamageComp = InVehicle->GetDamageComponent();
		if (!DamageComp)
		{
			C_Logger::Printf("Missing StandardDamageComponent on %x, cannot apply GodMode!", InVehicle);
			return false;
		}

		// Apply!
		DamageComp->SetInvincible(bGodModeActive);

		return true;
	}

	return false;
}

void ImGuiManager::SetPlayerFlyMode(bool bIsActive)
{
}

void ImGuiManager::ToggleFreezeLogic()
{
	bFreezeLogic = !bFreezeLogic;
	if (bFreezeLogic)
	{
		RWS::MainLoop::Logic::PushPause(16);
	}
	else
	{
		RWS::MainLoop::Logic::PopPause(16);
	}
}

void ImGuiManager::InitialiseNPCInspector(EARS::Modules::Sentient* InSentient, const bool bIsPlayer)
{
	CurrentInspector.Initialise(InSentient, bIsPlayer);
}

void ImGuiManager::ProcessBuildingTeleport()
{
	if (DeferredTeleportPayload.has_value())
	{
		const BuildingTeleportPayload& Payload = DeferredTeleportPayload.value();

		RwMatrixTag Transform;
		Transform.m_Pos = Payload.TeleportLocation;

		EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer();
		LocalPlayer->Teleport(Transform, 1152, nullptr, nullptr);

		DeferredTeleportPayload = {};
	}
}
