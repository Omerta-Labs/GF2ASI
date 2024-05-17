#include "GF2Hook.h"

#include "Addons/tConsole.h"
#include "Addons/tLog.h"

#include <detours.h>
#include <intrin.h>
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#include "Addons/Hook.h"

#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"

#include "SDK/EARS_Godfather/Modules/PartedModel/PartedModelMgr.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

#include "Scripthook/SH_ImGui/ImGuiManager.h"

// Disable all Multiplayer, not setup for GF2 Steam exe!
#define ENABLE_GF2_MULTIPLAYER 0

#if ENABLE_GF2_MULTIPLAYER
struct ConnectionParams
{
	const char* serverPartition;
	const char* clientVersion;
	unsigned int titleID;
	unsigned int port;
	const char* titleString;
	const char* sku;
	//Fesl::ServerInfo::FeslEnvironment environment;
	//const char* (__cdecl* localeCB)();
	//void(__cdecl* servicesCB)(Fesl::TitleParameters*);
	//void(__cdecl* listenersCB)(Fesl::ServiceHub*);
	//void(__cdecl* connGoneCB)(int);
};

uint64_t ConnectSocket_old;
typedef int(__thiscall* ConnectSocket)(void* pThis, void* fa, bool bUseSSL);
int __fastcall HOOK_ConnectSocket(void* pThis, void* ecx, void* fa, bool bUseSSL)
{
	C_Logger::Printf("Fesl::ConnectSocket");

	ConnectSocket funcCast = (ConnectSocket)ConnectSocket_old;
	const int value = funcCast(pThis, fa, bUseSSL);
	return value;
}

uint64_t DisonnectSocket_old;
typedef int(__thiscall* DisconnectSocket)(void* pThis);
int __fastcall HOOK_DisconnectSocket(void* pThis, void* ecx)
{
	C_Logger::Printf("Fesl::DisconnectSocket");

	DisconnectSocket funcCast = (DisconnectSocket)DisonnectSocket_old;
	const int value = funcCast(pThis);
	return value;
}

uint64_t Idle_old;
typedef int(__thiscall* Idle)(void* pThis, float curMilis);
int __fastcall HOOK_Idle(void* pThis, void* ecx, float curMilis)
{
	C_Logger::Printf("Fesl::Idle");

	Idle funcCast = (Idle)Idle_old;
	const int value = funcCast(pThis, curMilis);
	return value;
}

//uint64_t HelloReceived_old;
//typedef void(__thiscall* HelloReceived)(void* pThis, void* fa);
//void __fastcall HOOK_HelloReceived(void* pThis, void* ecx, void* fa)
//{
//	HelloReceived funcCast = (HelloReceived)HelloReceived_old;
//	funcCast(pThis, fa);
//}

uint64_t GodfatherConnectionManager_CTOR_old;
typedef void*(__thiscall* GodfatherConnectionManager_CTOR)(void* pThis, void* params, void* alloc);
void* __fastcall HOOK_GodfatherConnectionManager_CTOR(void* pThis, void* ecx, ConnectionParams* params, void* alloc)
{
	C_Logger::Printf("GodfatherConnectionManager_CTOR [%u -> %u]", params->port, 18020);

	params->port = 18020;
	//params->titleString = "gf2";

	GodfatherConnectionManager_CTOR funcCast = (GodfatherConnectionManager_CTOR)GodfatherConnectionManager_CTOR_old;
	auto value = funcCast(pThis, params, alloc);
	return value;
}

uint64_t ProtoAriesConnect_old;
int _cdecl HOOK_ProtoAriesConnect(uint32_t a1, const char* a2, uint32_t a3, uint32_t a4)
{
	const char* CustomFilter = "nephthys.xyz";

	C_Logger::Printf("ProtoAriesConnect [%s -> %s] [%u -> %u]", a2, CustomFilter, a3, 908500230);
	auto r = PLH::FnCast(ProtoAriesConnect_old, &HOOK_ProtoAriesConnect)(a1, CustomFilter, 908500230, a4);
	return r;
}

uint64_t HOOK_ProtoAriesPeek_old;
int _cdecl HOOK_ProtoAriesPeek(uint32_t a1, void* a2, int& a3, int& a4)
{
	auto r = PLH::FnCast(HOOK_ProtoAriesPeek_old, &HOOK_ProtoAriesPeek)(a1, a2, a3, a4);
	C_Logger::Printf("ProtoAriesPeek [%u -> %u]", a3, a4);

	return r;
}

uint64_t HOOK_ProtoAriesSend_old;
int _cdecl HOOK_ProtoAriesSend(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	auto r = PLH::FnCast(HOOK_ProtoAriesSend_old, &HOOK_ProtoAriesSend)(a1, a2, a3, a4, a5);
	C_Logger::Printf("!! ProtoAriesSend !!");

	return r;
}

uint64_t HOOK_ProtoAriesRecv_old;
int _cdecl HOOK_ProtoAriesRecv(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	auto r = PLH::FnCast(HOOK_ProtoAriesRecv_old, &HOOK_ProtoAriesRecv)(a1, a2, a3, a4, a5);
	C_Logger::Printf("!! ProtoAriesRecv !!");

	return r;
}
#endif // ENABLE_GF2_MULTIPLAYER

// STREAM MANAGER
uint64_t StreamManager_Load_Old;
typedef void* (__thiscall* StreamManager_Load)(void* pThis, const char*, float, uint32_t, void*, void*);
void* __fastcall HOOK_StreamManager_Load(void* pThis, void* ecx, const char* a1, float a2, uint32_t a3, void* a4, void* a5)
{
	C_Logger::Printf("StreamManager::Load");

	StreamManager_Load funcCast = (StreamManager_Load)StreamManager_Load_Old;
	auto value = funcCast(pThis, a1, a2, a3, a4, a5);
	return value;
}

// STREAM MANAGER
uint64_t Player_SetPlayerModel_Old;
typedef void(__thiscall* Player_SetPlayerModel)(EARS::Modules::Player* pThis, int ModelType);
void __fastcall HOOK_Player_SetPlayerModel(EARS::Modules::Player* pThis, void* ecx, int ModelType)
{
	Player_SetPlayerModel funcCast = (Player_SetPlayerModel)Player_SetPlayerModel_Old;
	funcCast(pThis, ModelType);

	pThis->m_CurrentModelType = ModelType;

	if (ModelType == 2)
	{
		MemUtils::CallClassMethod<void, EARS::Modules::Player*, void*, const char*>(0x09C58C0, pThis, nullptr, "unq_0005_miami");
	}
}

uint64_t Player_HandleEvents_Old;
typedef void(__thiscall* Player_HandleEvents)(EARS::Modules::Player* pThis, RWS::CMsg& MsgEvent);
void __fastcall HOOK_Player_HandleEvents(EARS::Modules::Player* pThis, void* ecx, RWS::CMsg& MsgEvent)
{
	Player_HandleEvents funcCast = (Player_HandleEvents)Player_HandleEvents_Old;
	funcCast(pThis, MsgEvent);

	pThis->HandleEvents(MsgEvent);
}

uint64_t Services_OpenLevelServices_Old;
typedef void(__thiscall* Services_OpenLevelServices)(void* pThis);
void __fastcall HOOK_Services_OpenLevelServices(void* pThis, void* ecx)
{
	Services_OpenLevelServices funcCast = (Services_OpenLevelServices)Services_OpenLevelServices_Old;
	funcCast(pThis);
}

uint64_t GodfatherBaseServices_HandleEvents_Old;
typedef void(__thiscall* GodfatherBaseServices_HandleEvents)(void* pThis, const RWS::CMsg& MsgEvent);
void __fastcall HOOK_GodfatherBaseServices_HandleEvents(void* pThis, void* ecx, const RWS::CMsg& MsgEvent)
{
	GodfatherBaseServices_HandleEvents funcCast = (GodfatherBaseServices_HandleEvents)GodfatherBaseServices_HandleEvents_Old;
	funcCast(pThis, MsgEvent);

	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	if (MsgEvent.IsEvent(RunningTickEvent))
	{
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
			{
				LocalPlayer->TrySwapPlayerModel();
			}
		}
	}
}

static bool show_demo_window = false;
static bool show_parted_model_window = false;
static bool bTakeoverCursor = false;

/**
 * DISPL
 */
uint64_t Displ_BeginScene_Old;
void __fastcall HOOK_Displ_BeginScene()
{
	ImGuiIO& IO = ImGui::GetIO();
	IO.MouseDrawCursor = bTakeoverCursor;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (show_parted_model_window)
	{
		PartedModelMgr* ModelMgr = PartedModelMgr::GetInstance();
		if (ModelMgr == nullptr)
		{
			// not loaded yet so don't show anything
			return;
		}

		if (ImGui::Begin("Parted Model Manager", &show_parted_model_window))
		{
			ImGui::Text("Parted Model List");
			ImGui::BeginChild("parted_model_list");

			PartedModel::AssemblyListHeader* CurrentAssembly = ModelMgr->m_AssembliesList;
			while (CurrentAssembly != nullptr)
			{
				for (uint32_t i = 0; i < CurrentAssembly->m_NumAssemblies; i++)
				{
					const PartedModel::Assembly& ThisAssembly = CurrentAssembly->m_Assemblies[i];
					ImGui::Text("%s [%s]", ThisAssembly.m_AssemblyName, (ThisAssembly.m_Flags == 4 ? "VEHICLE" : "CHARACTER"));

					if (ThisAssembly.m_AssemblyName == "unq_fredo_corleone")
					{
						int z = 0;
					}
				}

				CurrentAssembly = CurrentAssembly->m_NextHeader;
			}

			ImGui::EndChild();
			ImGui::End();
		}
	}

	ImGui::EndFrame();

	PLH::FnCast(Displ_BeginScene_Old, &HOOK_Displ_BeginScene)();
}

uint64_t Displ_EndScene_Old;
void __fastcall HOOK_Displ_EndScene()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	PLH::FnCast(Displ_EndScene_Old, &HOOK_Displ_EndScene)();
}

uint64_t WinProc_GF2_Old;
int __stdcall WndProc_GF2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Let ImGui add inputs
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
	{
		return true;
	}

	return PLH::FnCast(WinProc_GF2_Old, &WndProc_GF2)(hWnd, Msg, wParam, lParam);
}

uint64_t SetCursorPos_old;
void HOOK_SetCursorPos(int x, int y)
{
	if (bTakeoverCursor)
	{
		// TODO: We can do better than this
		// avoid the game from forcing the mouse to the centre
		return;
	}

	PLH::FnCast(SetCursorPos_old, &HOOK_SetCursorPos)(x, y);
}

void GF2Hook::Init()
{
	//tConsole::fCreate("Godfather II: ASI Hook");
	C_Logger::Create("GF2_Hook.txt");

	//tConsole::fWriteLine("Welcome to GF2 hook");

	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	hook::Type<HWND> windowHandle = hook::Type<HWND>(0x112A024);
	ImGui_ImplWin32_Init(windowHandle);

	hook::Type<IDirect3DDevice9*> class_index = hook::Type<IDirect3DDevice9*>(0x1205750);
	ImGui_ImplDX9_Init(class_index);

#if ENABLE_GF2_MULTIPLAYER
	// TODO: Adjust all addresses to use Steam EXE addresses
	PLH::x86Detour detour100((char*)0x0C6B2C0, (char*)&HOOK_ProtoAriesConnect, &ProtoAriesConnect_old, dis);
	detour100.hook();

	PLH::x86Detour detour110((char*)0xC6B190, (char*)&HOOK_ProtoAriesPeek, &HOOK_ProtoAriesPeek_old, dis);
	detour110.hook();

	PLH::x86Detour detour111((char*)0x0C6B080, (char*)&HOOK_ProtoAriesSend, &HOOK_ProtoAriesSend_old, dis);
	detour111.hook();

	PLH::x86Detour detour112((char*)0xC6B250, (char*)&HOOK_ProtoAriesRecv, &HOOK_ProtoAriesRecv_old, dis);
	detour112.hook();

	PLH::x86Detour detour101((char*)0xADAC10, (char*)&HOOK_ConnectSocket, &ConnectSocket_old, dis);
	detour101.hook();

	PLH::x86Detour detour104((char*)0x0ADAB80, (char*)&HOOK_DisconnectSocket, &DisonnectSocket_old, dis);
	detour104.hook();

	PLH::x86Detour detour105((char*)0x0ADAC80, (char*)&HOOK_Idle, &Idle_old, dis);
	detour105.hook();

	PLH::x86Detour detour102((char*)0x08A1C80, (char*)&HOOK_GodfatherConnectionManager_CTOR, &GodfatherConnectionManager_CTOR_old, dis);
	detour102.hook();

	PLH::x86Detour detour155((char*)0x403A30, (char*)&HOOK_StreamManager_Load, &StreamManager_Load_Old, dis);
	detour155.hook();

	PLH::x86Detour detour103((char*)0x0AEA650, (char*)&HOOK_HelloReceived, &HelloReceived_old, dis);
	detour103.hook();
#endif // ENABLE_GF2_MULTIPLAYER

	PLH::x86Detour detour102((char*)0x608620, (char*)&HOOK_Displ_BeginScene, &Displ_BeginScene_Old, dis);
	detour102.hook();

	PLH::x86Detour detour155((char*)0x608930, (char*)&HOOK_Displ_EndScene, &Displ_EndScene_Old, dis);
	detour155.hook();

	PLH::x86Detour detour157((char*)0x0403A50, (char*)&HOOK_StreamManager_Load, &StreamManager_Load_Old, dis);
	detour157.hook();

	PLH::x86Detour detour171((char*)0x07A60C0, (char*)&HOOK_Player_SetPlayerModel, &Player_SetPlayerModel_Old, dis);
	detour171.hook();

	PLH::x86Detour detour173((char*)0x07A93A0, (char*)&HOOK_Player_HandleEvents, &Player_HandleEvents_Old, dis);
	detour173.hook();

	PLH::x86Detour detour1733((char*)0x0680EF0, (char*)&HOOK_Services_OpenLevelServices, &Services_OpenLevelServices_Old, dis);
	detour1733.hook();

	PLH::x86Detour detour17343((char*)0x8F6CE0, (char*)&HOOK_GodfatherBaseServices_HandleEvents, &GodfatherBaseServices_HandleEvents_Old, dis);
	detour17343.hook();

	PLH::x86Detour detour159((char*)0x69DE10, (char*)&WndProc_GF2, &WinProc_GF2_Old, dis);
	detour159.hook();

	PLH::x86Detour detour172((char*)0x69E840, (char*)&HOOK_SetCursorPos, &SetCursorPos_old, dis);
	detour172.hook();
}

static bool bIsCursorVisible = false;
void GF2Hook::Tick()
{
	if (GetAsyncKeyState(VK_F1) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		show_parted_model_window = !show_parted_model_window;
	}

	if (GetAsyncKeyState(VK_F2) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		show_demo_window = !show_demo_window;
	}

	// Update cursor visibility
	// Should only really be present when any ImGui windows are open - 
	// The ingame cursor (for menus) is expected to be powered by Apt.
	const bool bCursorVisibilityThisFrame = show_demo_window || show_parted_model_window;
	if (bCursorVisibilityThisFrame != bIsCursorVisible)
	{
		bIsCursorVisible = bCursorVisibilityThisFrame;
		bTakeoverCursor = bIsCursorVisible;
	}
}

