#include "GF2Hook.h"

#include "Addons/tConsole.h"
#include "Addons/tLog.h"

#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#include "Addons/imgui/backends/imgui_impl_win32.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"

#include "Scripthook/SH_ImGui/ImGuiManager.h"
#include "Scripthook/SH_Discord/DiscordManager.h"

#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegion.h"

// Disable all Multiplayer, not setup for GF2 Steam exe!
#define ENABLE_GF2_MULTIPLAYER 0
#define ENABLE_GF2_DISPL_BEGINSCENE_HOOK 0
#define ENABLE_GF2_GODFATHER_SERVICES_TICK_HOOK 0
#define ENABLE_GF2_SPAWN_ENTITY_HOOKS 0

ImGuiManager* OurImGuiManager = nullptr;
DiscordManager* OurDiscordManager = nullptr;

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

/**
 * Hook to dump which Stream files are loaded
 * (Doesn't actually do that though)
 */
uint64_t StreamManager_Load_Old;
typedef void* (__thiscall* StreamManager_Load)(void* pThis, const char*, float, uint32_t, void*, void*);
void* __fastcall HOOK_StreamManager_Load(void* pThis, void* ecx, const char* a1, float a2, uint32_t a3, void* a4, void* a5)
{
	C_Logger::Printf("StreamManager::Load");

	StreamManager_Load funcCast = (StreamManager_Load)StreamManager_Load_Old;
	auto value = funcCast(pThis, a1, a2, a3, a4, a5);
	return value;
}

#if ENABLE_GF2_GODFATHER_SERVICES_TICK_HOOK
/**
 * Hook to allow our systems to receive a tick
 * Good example is ImGui, so we can trigger bespoke behaviour.
 * Unfortunately we cannot replicate their approach (a class with an RWS::CEventHandler)
 */
uint64_t GodfatherBaseServices_HandleEvents_Old;
typedef void(__thiscall* GodfatherBaseServices_HandleEvents)(void* pThis, const RWS::CMsg& MsgEvent);
void __fastcall HOOK_GodfatherBaseServices_HandleEvents(void* pThis, void* ecx, const RWS::CMsg& MsgEvent)
{
	GodfatherBaseServices_HandleEvents funcCast = (GodfatherBaseServices_HandleEvents)GodfatherBaseServices_HandleEvents_Old;
	funcCast(pThis, MsgEvent);

	// Piggy back of the Godfather base services, 
	// for some reason we cant create our own event handler as of yet
	//OurImGuiManager.HandleEvents(MsgEvent);
	//OurDiscordManager.HandleEvents(MsgEvent);
}
#endif // ENABLE_GF2_GODFATHER_SERVICES_TICK_HOOK

#if ENABLE_GF2_DISPL_BEGINSCENE_HOOK
/**
 * Not used for anything anymore;
 * Keeping around (BUT UNHOOKED) if it ever is required
 */
uint64_t Displ_BeginScene_Old;
void __cdecl HOOK_Displ_BeginScene()
{
	PLH::FnCast(Displ_BeginScene_Old, &HOOK_Displ_BeginScene)();
}
#endif // ENABLE_GF2_DISPL_BEGINSCENE_HOOK

/**
 * Used to implement the 'end scene' call for ImGuiManager
 */
uint64_t Displ_EndScene_Old;
void __cdecl HOOK_Displ_EndScene()
{
	if (OurImGuiManager)
	{
		OurImGuiManager->OnEndScene();
	}

	PLH::FnCast(Displ_EndScene_Old, &HOOK_Displ_EndScene)();
}

/**
 * Used to implement reset for ImGui
 * Resolve crash when anything resets the DX9 device
 */
uint64_t Displ_ResetDevice_Old;
bool __cdecl HOOK_Displ_ResetDevice(int a1)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const bool bResult = PLH::FnCast(Displ_ResetDevice_Old, &HOOK_Displ_ResetDevice)(a1);

	if (bResult)
	{
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	return bResult;
}

/**
 * Allows ImGui the ability to react to Window messages
 */
uint64_t WinProc_GF2_Old;
int __stdcall WndProc_GF2(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// Let ImGui add inputs
	if (OurImGuiManager && OurImGuiManager->WndProc(hWnd, Msg, wParam, lParam))
	{
		return true;
	}

	return PLH::FnCast(WinProc_GF2_Old, &WndProc_GF2)(hWnd, Msg, wParam, lParam);
}

/**
 * Allows ImGui to disable the cursor from automatically being recentered
 */
uint64_t SetCursorPos_old;
void HOOK_SetCursorPos(int x, int y)
{
	if (OurImGuiManager && OurImGuiManager->HasCursorControl())
	{
		// TODO: We can do better than this
		// avoid the game from forcing the mouse to the centre
		return;
	}

	PLH::FnCast(SetCursorPos_old, &HOOK_SetCursorPos)(x, y);
}

#if ENABLE_GF2_SPAWN_ENTITY_HOOKS
uint64_t SimManager_GetAttributePacket_Old;
typedef void* (__thiscall* SimManager_GetAttributePacket)(void*, EARS::Common::guid128_t&, int);
void* __fastcall HOOK_SimManager_GetAttributePacket(void* pThis, void* ecx, EARS::Common::guid128_t& InGuid, int InMask)
{
	SimManager_GetAttributePacket funcCast = (SimManager_GetAttributePacket)SimManager_GetAttributePacket_Old;
	auto value = funcCast(pThis, InGuid, InMask);
	return value;
}

uint64_t SimManager_SpawnEntity_Old;
typedef void* (__thiscall* SimManager_SpawnEntity)(void*, void*, int, bool);
void* __fastcall HOOK_SimManager_SpawnEntity(void* pThis, void* ecx, void* Packet, int a2, bool a3)
{
	SimManager_SpawnEntity funcCast = (SimManager_SpawnEntity)SimManager_SpawnEntity_Old;
	auto value = funcCast(pThis, Packet, a2, a3);
	return value;
}

uint64_t NPCManager_Create_Old;
typedef void* (__thiscall* NPCManager_Create)(void*, const EARS::Common::guid128_t& InGuid, uint32_t InPriority, void* InOwner, uint32_t InHStream);
void* __fastcall HOOK_NPCManager_Create(void* pThis, void* ecx, const EARS::Common::guid128_t& InGuid, uint32_t InPriority, void* InOwner, uint32_t InHStream)
{
	NPCManager_Create funcCast = (NPCManager_Create)SimManager_GetAttributePacket_Old;
	auto value = funcCast(pThis, InGuid, InPriority, InOwner, InHStream);
	return value;
}
#endif // ENABLE_GF2_SPAWN_ENTITY_HOOKS

uint64_t OpenLevelServices_Old;
void __cdecl Hook_OpenLevelServices()
{
	PLH::FnCast(OpenLevelServices_Old, &Hook_OpenLevelServices)();

	OurImGuiManager = new ImGuiManager();
	OurImGuiManager->Open();

	OurDiscordManager = new DiscordManager();
	OurDiscordManager->Open();
}

uint64_t CloseLevelServices_Old;
void __cdecl Hook_CloseLevelServices()
{
	if (OurImGuiManager)
	{
		delete OurImGuiManager;
		OurImGuiManager = nullptr;
	}

	if (OurDiscordManager)
	{
		delete OurDiscordManager;
		OurDiscordManager = nullptr;
	}

	PLH::FnCast(CloseLevelServices_Old, &Hook_CloseLevelServices)();
}

void GF2Hook::Init()
{
	C_Logger::Create("GF2_Hook.txt");

	PLH::ZydisDisassembler dis(PLH::Mode::x86);

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

#if ENABLE_GF2_DISPL_BEGINSCENE_HOOK
	PLH::x86Detour detour102((char*)0x608620, (char*)&HOOK_Displ_BeginScene, &Displ_BeginScene_Old, dis);
	detour102.hook();
#endif // ENABLE_GF2_DISPL_BEGINSCENE_HOOK

	PLH::x86Detour detour155((char*)0x608930, (char*)&HOOK_Displ_EndScene, &Displ_EndScene_Old, dis);
	detour155.hook();

	PLH::x86Detour detour125((char*)0x608250, (char*)&HOOK_Displ_ResetDevice, &Displ_ResetDevice_Old, dis);
	detour125.hook();

	PLH::x86Detour detour157((char*)0x0403A50, (char*)&HOOK_StreamManager_Load, &StreamManager_Load_Old, dis);
	detour157.hook();

#if ENABLE_GF2_GODFATHER_SERVICES_TICK_HOOK
	PLH::x86Detour detour17343((char*)0x8F6CE0, (char*)&HOOK_GodfatherBaseServices_HandleEvents, &GodfatherBaseServices_HandleEvents_Old, dis);
	detour17343.hook();
#endif // ENABLE_GF2_GODFATHER_SERVICES_TICK_HOOK

	PLH::x86Detour detour159((char*)0x69DE10, (char*)&WndProc_GF2, &WinProc_GF2_Old, dis);
	detour159.hook();

	PLH::x86Detour detour172((char*)0x69E840, (char*)&HOOK_SetCursorPos, &SetCursorPos_old, dis);
	detour172.hook();

#if ENABLE_GF2_SPAWN_ENTITY_HOOKS
	PLH::x86Detour detour175((char*)0x04461C0, (char*)&HOOK_SimManager_GetAttributePacket, &SimManager_GetAttributePacket_Old, dis);
	detour175.hook();

	// TODO: Disabled because this causes issues
	//PLH::x86Detour detour177((char*)0x08F0BB0, (char*)&HOOK_NPCManager_Create, &NPCManager_Create_Old, dis);
	//detour177.hook();

	PLH::x86Detour detour1780((char*)0x0446340, (char*)&HOOK_SimManager_SpawnEntity, &SimManager_SpawnEntity_Old, dis);
	detour1780.hook();
#endif // ENABLE_GF2_SPAWN_ENTITY_HOOKS

	PLH::x86Detour detour178((char*)0x06817C0, (char*)&Hook_OpenLevelServices, &OpenLevelServices_Old, dis);
	detour178.hook();

	PLH::x86Detour detour221((char*)0x0682860, (char*)&Hook_CloseLevelServices, &CloseLevelServices_Old, dis);
	detour221.hook();

	EARS::Modules::DemographicRegion::StaticApplyHooks();
}

void GF2Hook::Tick()
{
	//Nothing
}
