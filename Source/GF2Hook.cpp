#include "GF2Hook.h"

#include "Addons/tConsole.h"
#include "Addons/tLog.h"
#include "Addons/Settings.h"

#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#include "Addons/imgui/backends/imgui_impl_win32.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"

#include "Scripthook/ScripthookEvents.h"
#include "Scripthook/SH_ImGui/ImGuiManager.h"
#include "Scripthook/SH_Discord/DiscordManager.h"
#include "Scripthook/SH_ObjectManager/ObjectManager.h"
#include "Scripthook/SH_PlayerMasterSM/PlayerDebugOptions_Modded.h"
#include "Scripthook/SH_GammaFix/GammaFix.h"
#include "Scripthook/SH_EdgeAA/EdgeAA.h"
#include "Scripthook/SH_ModManager/ModManager.h"
#include "Scripthook/HookMods.h"

#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Framework/Core/SimManager/SimManager.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"
#include "SDK/EARS_Godfather/Modules/PartedAnimated/PartedAnimated.h"
#include "SDK/EARS_Godfather/Modules/Families/CorleoneData.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegion.h"
#include "SDK/EARS_Godfather/Modules/Scoring/ScoreKeeper.h"
#include "SDK/EARS_Godfather/Modules/Mobface/MobfaceManager.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/Trinity/TrinityGameCamera.h"
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include <SDK/EARS_RT_LLRender/include/ShaderManager.h>

#include <sol.hpp>
#include <thread>
#include <Addons/Hook.h>

// Disable all Multiplayer, not setup for GF2 Steam exe!
#define ENABLE_GF2_DISPL_BEGINSCENE_HOOK 0
#define ENABLE_GF2_GODFATHER_SERVICES_TICK_HOOK 0
#define ENABLE_GF2_SPAWN_ENTITY_HOOKS 0
#define ENABLE_GF2_SIM_MANAGER_RE 1

/*
template<typename TType, uint32_t N>
struct AptValueSet
{
public:

	int32_t m_NumElements = 0;
	TType m_Elements[512];
};

struct AptCXForm
{
	float scale[4];
	float translate[4];
};

struct AptMatrix
{
	float a;
	float b;
	float c;
	float d;
	float tx;
	float ty;
};

struct AptValue
{
public:

	uint32_t m_ValueData = 0;
};

struct AptString : public AptValue
{
	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;
	char* m_String = nullptr;
	char m_InlineString[16];
};

struct AptCIH : public AptValue
{
	__declspec(align(16)) AptCXForm cxform;
	int m_Depth = 0;
	AptString* m_MyName = nullptr;
	AptMatrix matrix;
	void* m_ProceduralSettings = nullptr;
	AptCIH* m_Parent = nullptr;
	void* m_Data = nullptr;
	AptCIH* m_Prev = nullptr;
	AptCIH* m_Next = nullptr;
	bool m_bASChange = false;
};

struct AptDisplayListState
{
	AptCIH* m_Head;
};

struct AptDisplayList
{
	AptDisplayListState* m_State = nullptr;
};

struct AptAnimationPoolData
{
public:

	struct AptActionPool
	{
		uint32_t m_ActionType = 0;
		uint32_t m_Input = 0;
		char m_Padding[0xC];
	};

	AptAnimationPoolData::AptActionPool m_ActionPool[256];
	AptAnimationPoolData::AptActionPool* m_StartDeque = nullptr;
	AptAnimationPoolData::AptActionPool* m_EndDeque = nullptr;
	AptCIH* m_NewInsts[512];
	int32_t m_NumNewInsts = 0;
	AptValueSet<AptValue*, 64> m_ListenerSet;
	AptValueSet<AptCIH*, 512> m_InputSet;
	AptDisplayList m_DisplayList;

};

struct AptHashItem
{
public:

	AptString* m_Key = nullptr;
	AptValue* m_Value = nullptr;
	AptHashItem* m_Next = nullptr;
	uint32_t m_HashValue = 0;
};

struct AptNativeHash
{
public:

	uint16_t m_Size = 0;
	uint16_t m_NumItems = 0;
	AptHashItem** m_Data = nullptr;
	AptHashItem* m_InlineData[16];
};

struct AptGlobal : public AptValue, public AptNativeHash
{

};
*/

/**
 * Hook on StreamManager::Load (by name). Logs stream requests and gives the
 * ModManager its window to apply stream patches: replacements are registered
 * before the first load resolves, additions are queued once the first game
 * load has been issued (mirrors GFStreamAdditionHandler timing).
 */
uint64_t StreamManager_Load_Old;
typedef void* (__thiscall* StreamManager_Load)(void* pThis, const char*, float, uint32_t, void*, void*);
void* __fastcall HOOK_StreamManager_Load(void* pThis, void* ecx, const char* a1, float a2, uint32_t a3, void* a4, void* a5)
{
	tConsole::fPrintf("StreamManager::Load [%s]", a1);

	EARS::Framework::StreamManager* StreamMgr = reinterpret_cast<EARS::Framework::StreamManager*>(pThis);
	SH::ModManager::GetInstance().OnStreamManagerLoadBegin(StreamMgr);

	StreamManager_Load funcCast = (StreamManager_Load)StreamManager_Load_Old;
	auto value = funcCast(pThis, a1, a2, a3, a4, a5);

	SH::ModManager::GetInstance().OnStreamManagerLoadEnd(StreamMgr);
	return value;
}

uint64_t StreamManager_LoadHierarchy_Old;
typedef void* (__thiscall* StreamManager_LoadHierarchy)(void* pThis, EARS::Framework::StreamFile*, float, const char*, uint32_t);
void* __fastcall HOOK_StreamManager_LoadHierarchy(void* pThis, void* ecx, EARS::Framework::StreamFile* stream, float priority, const char* dlPrefix, uint32_t loadFlags)
{
	if (stream)
	{
		tConsole::fPrintf("StreamManager::LoadHierarchy [0x%X] - [%s]", stream->GetGUID(), stream->GetFileName());
	}

	EARS::Framework::StreamManager* Mgr = EARS::Framework::StreamManager::GetInstance();

	StreamManager_LoadHierarchy funcCast = (StreamManager_LoadHierarchy)StreamManager_LoadHierarchy_Old;
	auto value = funcCast(pThis, stream, priority, dlPrefix, loadFlags);

	return value;
}

uint64_t SimManager_LoadResource_Old;
typedef void (__thiscall* SimManager_LoadResource)(EARS::Framework::SimManager* pThis, RWS::CResourceHandler::CResourceLoadInfo*);
void __fastcall HOOK_SimManager_LoadResource(EARS::Framework::SimManager* pThis, void* ecx, RWS::CResourceHandler::CResourceLoadInfo* pLoadInfo)
{
#if ENABLE_GF2_SIM_MANAGER_RE
	// pThis is not correct pointer, appears to be vtable version for ResourceHandler
	EARS::Framework::SimManager* ResolvedSimMgr = EARS::Framework::SimManager::GetInstance();
	ResolvedSimMgr->LoadResource(pLoadInfo);
#else // ENABLE_GF2_SIM_MANAGER_RE
	SimManager_LoadResource funcCast = (SimManager_LoadResource)SimManager_LoadResource_Old;
	funcCast(pThis, pLoadInfo);
#endif // ENABLE_GF2_SIM_MANAGER_RE
}

uint64_t MobfaceManager_SetTargetModel_Old;
typedef void (__thiscall* MobfaceManager_SetTargetModel)(void*, EARS::Modules::PartedAnimated*);
void __fastcall HOOK_MobfaceManager_SetTargetModel(void* pThis, void* ecx, EARS::Modules::PartedAnimated* InModel)
{
	/*
	// NB: This is the code to fix crashing when entering mobface and apparel editor with mobface people when the player model has been swapped.
	// This works as MVP but still has bugs:
	// - Hats will still show regardless of hidden state
	// - Crew members can end up with no bodies, no clothes etc
	// - Initial Mobface is broken too, with no bodies or clothes.
	// We need to fix these bugs before this can be enabled.

	// ensure we have dependencies
	EARS::Modules::MobfaceManager* MobfaceMgr = EARS::Modules::MobfaceManager::GetInstance();
	assert(MobfaceMgr);

	EARS::Modules::PlayerFamilyTree* PlayerTree = EARS::Modules::PlayerFamilyTree::GetInstance();
	assert(PlayerTree);

	EARS::Modules::PlayerFamilyTree::FamilyTreeSlot TargetSlot = EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_PLAYER;

	if (const EARS::Modules::NPC* const AsNPC = EARS::Framework::_QueryInterface<EARS::Modules::NPC>(InModel, 0x369AC561))
	{
		const EARS::Modules::SimNPC* const OwnSimNPC = AsNPC->GetOwningSimNPC();
		TargetSlot = PlayerTree->FindTreeSlotIndex(OwnSimNPC);
	}

	if(MobfaceMgr->HasSavedData(TargetSlot))
	{
		MobfaceMgr->BuildModelFromSavedData(TargetSlot, InModel);
	}
	*/

	MobfaceManager_SetTargetModel funcCast = (MobfaceManager_SetTargetModel)MobfaceManager_SetTargetModel_Old;
	funcCast(pThis, InModel);
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
	if (ImGuiManager* ImGuiMgr = ImGuiManager::Get())
	{
		ImGuiMgr->OnEndScene();
	}

	// Keep the EdgeAA pass armed for the next frame's post-FX chain
	Mod::EdgeAA::OnEndScene();

	// Windowed-mode brightness: applied last so the whole frame is affected,
	// matching how the hardware gamma ramp behaves in fullscreen
	Mod::GammaFix::OnEndScene();

	PLH::FnCast(Displ_EndScene_Old, &HOOK_Displ_EndScene)();
}

/**
 * Used to implement reset for ImGui
 * Resolve crash when anything resets the DX9 device
 */
uint64_t Displ_ResetDevice_Old;
bool __cdecl HOOK_Displ_ResetDevice(int a1)
{
	if (ImGuiManager* ImGuiMgr = ImGuiManager::Get())
	{
		ImGuiMgr->OnDeviceLost();
	}

	Mod::GammaFix::OnDeviceReset();

	const bool bResult = PLH::FnCast(Displ_ResetDevice_Old, &HOOK_Displ_ResetDevice)(a1);

	if (bResult)
	{
		if (ImGuiManager* ImGuiMgr = ImGuiManager::Get())
		{
			ImGuiMgr->OnDeviceRestored();
		}
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
	if (ImGuiManager* ImGuiMgr = ImGuiManager::Get())
	{
		if (ImGuiMgr->WndProc(hWnd, Msg, wParam, lParam))
		{
			return true;
		}
	}

	return PLH::FnCast(WinProc_GF2_Old, &WndProc_GF2)(hWnd, Msg, wParam, lParam);
}

/**
 * Allows ImGui to disable the cursor from automatically being recentered
 */
uint64_t SetCursorPos_old;
void HOOK_SetCursorPos(int x, int y)
{
	if (const ImGuiManager* ImGuiMgr = ImGuiManager::Get())
	{
		if (ImGuiMgr->HasCursorControl())
		{
			// TODO: We can do better than this
			// avoid the game from forcing the mouse to the centre
			return;
		}
	}

	PLH::FnCast(SetCursorPos_old, &HOOK_SetCursorPos)(x, y);
}

#if ENABLE_GF2_SPAWN_ENTITY_HOOKS
uint64_t SimManager_GetAttributePacket_Old;
typedef void* (__thiscall* SimManager_GetAttributePacket)(EARS::Framework::SimManager*, EARS::Common::guid128_t&, int);
void* __fastcall HOOK_SimManager_GetAttributePacket(EARS::Framework::SimManager* pThis, void* ecx, EARS::Common::guid128_t& InGuid, int InMask)
{
	SimManager_GetAttributePacket funcCast = (SimManager_GetAttributePacket)SimManager_GetAttributePacket_Old;
	auto value = funcCast(pThis, InGuid, InMask);
	return value;
}

uint64_t SimManager_SpawnEntity_Old;
typedef void* (__thiscall* SimManager_SpawnEntity)(EARS::Framework::SimManager*, RWS::CAttributePacket*, int, bool);
void* __fastcall HOOK_SimManager_SpawnEntity(EARS::Framework::SimManager* pThis, void* ecx, RWS::CAttributePacket* Packet, int a2, bool a3)
{
	EARS::Framework::StreamManager* StreamMgr = EARS::Framework::StreamManager::GetInstance();

	const EARS::Common::guid128_t PacketID = Packet->GetInstanceID();
	const uint32_t ClassID = Packet->GetIdOfClassToCreate();
	const uint32_t StreamHdl = Packet->GetStreamHandle();

	EARS::Framework::Stream* Str = StreamMgr->GetStreamFromHandle(StreamHdl);
	const char* StrFilename = Str->GetFileName();

	tConsole::fPrintf("[%s] -> %u -> %u %u %u %u", StrFilename, PacketID, PacketID.a, PacketID.b, PacketID.c, PacketID.d);

	SimManager_SpawnEntity funcCast = (SimManager_SpawnEntity)SimManager_SpawnEntity_Old;
	auto value = funcCast(pThis, Packet, a2, a3);
	return value;
}

uint64_t City_HandleAttributes_Old;
typedef void(__thiscall* City_HandleAttributes)(EARS::Modules::City*, RWS::CAttributePacket*);
void __fastcall HOOK_City_HandleAttributes(EARS::Modules::City* pThis, void* ecx, RWS::CAttributePacket* InAttr)
{
	City_HandleAttributes funcCast = (City_HandleAttributes)City_HandleAttributes_Old;
	funcCast(pThis, InAttr);
}
#endif // ENABLE_GF2_SPAWN_ENTITY_HOOKS

uint64_t EARSJobScheduler_GetDefaultThreadCount_old;
int _cdecl HOOK_EARSJobScheduler_GetDefaultThreadCount()
{
	if (Settings::GetCheckedRef().ApplyCPUFix())
	{
		// Returns logical processor count (includes hyperthreads)
		unsigned int coreCount = std::thread::hardware_concurrency();

		const int UpperCoreCountLimit = min(10, coreCount);
		const int LowerCoreCountLimit = max(0, UpperCoreCountLimit - 2);
		return LowerCoreCountLimit;
	}
	else
	{
		return PLH::FnCast(EARSJobScheduler_GetDefaultThreadCount_old, &HOOK_EARSJobScheduler_GetDefaultThreadCount)();
	}
}

uint64_t OpenLevelServices_Old;
void __cdecl Hook_OpenLevelServices()
{
	PLH::FnCast(OpenLevelServices_Old, &Hook_OpenLevelServices)();

	// PURPOSE: Implement PlayerDebugOptions for Scripthook
	//			This includes original PlayerDebugOptions and Scripthook version.
	SH::PlayerDebugOptions_Modded* NewOptions = new SH::PlayerDebugOptions_Modded();

	//TestLSShader();
	//TestLSShaderData();

	// PURPOSE: Update corleone family data to consider whether the user wants DLC content
	if (EARS::Modules::CorleoneFamilyData* FamilyData = EARS::Modules::CorleoneFamilyData::GetInstance())
	{
		const Settings& OurSettings = Settings::GetCheckedRef();
		if (OurSettings.WantsPreOrderBonus())
		{
			FamilyData->UnlockPreOrderCrew();
		}
		else
		{
			FamilyData->LockPreOrderCrew();
		}
	}

	const Settings& HookSettings = Settings::GetCheckedRef();

	if (Mod::ObjectManager* ObjectMgr = Mod::ObjectManager::Get())
	{
		// create a new object manager
	}

	if (ImGuiManager* ImGuiMgr = ImGuiManager::Get())
	{
		ImGuiMgr->OpenLevelServices();
	}

	if (DiscordManager* OurDiscordManager = DiscordManager::Get())
	{
		OurDiscordManager->OpenLevelServices();
	}

}

uint64_t CloseLevelServices_Old;
void __cdecl Hook_CloseLevelServices()
{
	if (DiscordManager* OurDiscordManager = DiscordManager::Get())
	{
		OurDiscordManager->CloseLevelServices();
		DiscordManager::DestroyInstance();
	}

	if (Mod::ObjectManager* OwnObjectMgr = Mod::ObjectManager::Get())
	{
		Mod::ObjectManager::DestroyInstance();
	}

	if (ImGuiManager* ImGuiMgr = ImGuiManager::Get())
	{
		ImGuiMgr->CloseLevelServices();
	}

	// PURPOSE: Implement PlayerDebugOptions from xbox
	if (EARS::Modules::PlayerDebugOptions* DebugOptions = EARS::Modules::PlayerDebugOptions::GetInstance())
	{
		delete DebugOptions;
		DebugOptions = nullptr;
	}

	PLH::FnCast(CloseLevelServices_Old, &Hook_CloseLevelServices)();
}

uint64_t TrinityGameCamera_BeginUpdate_Old;
typedef void (__thiscall* TrinityGameCamera_BeginUpdate)(EARS::Modules::TrinityGameCamera*);
void __fastcall HOOK_TrinityGameCamera_BeginUpdate(EARS::Modules::TrinityGameCamera* pThis, void* ecx)
{
	// pThis points at the EA::Trinity::TrinityCamera subobject; step back to the full object
	EARS::Modules::TrinityGameCamera* ActualPtr = (EARS::Modules::TrinityGameCamera*)((uint8_t*)pThis - 12);

	// Call our reimplementation directly (qualified to avoid dispatching back into the game's vtable)
	ActualPtr->EARS::Modules::TrinityGameCamera::BeginUpdate();
}

void GF2Hook::Init_Logging()
{
	C_Logger::Create("GF2SE.txt");
#if DEBUG
	tConsole::fCreate("GF2SE");
#endif // DEBUG

	tConsole::RouteToLogger(true);
}

void GF2Hook::Init_ModSystems()
{
	Settings* SettingsMgr = Settings::Get();
	SettingsMgr->Init();

	EARS::Framework::InitialiseScripthookModLoader();
}

void GF2Hook::Init_AttachHooks()
{
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	Mod::ApplyHooks();

#if ENABLE_GF2_MULTIPLAYER
	// TODO: Adjust all addresses to use Steam EXE addresses
	PLH::x86Detour detour100((char*)0x0C7E2E0, (char*)&HOOK_ProtoAriesConnect, &ProtoAriesConnect_old, dis);
	detour100.hook();

	PLH::x86Detour detour110((char*)0x0C7E1B0, (char*)&HOOK_ProtoAriesPeek, &HOOK_ProtoAriesPeek_old, dis);
	detour110.hook();

	PLH::x86Detour detour111((char*)0x0C7E0A0, (char*)&HOOK_ProtoAriesSend, &HOOK_ProtoAriesSend_old, dis);
	detour111.hook();

	PLH::x86Detour detour112((char*)0x0C7E270, (char*)&HOOK_ProtoAriesRecv, &HOOK_ProtoAriesRecv_old, dis);
	detour112.hook();

	PLH::x86Detour detour122((char*)0x0C8C830, (char*)&HOOK_ProtoSSLRecv, &HOOK_ProtoSSLRecv_old, dis);
	detour122.hook();

	PLH::x86Detour detour123((char*)0x0C8C790, (char*)&HOOK_ProtoSSLSend, &HOOK_ProtoSSLSend_old, dis);
	detour123.hook();

	PLH::x86Detour detour101((char*)0x0AEDD10, (char*)&HOOK_ConnectSocket, &ConnectSocket_old, dis);
	detour101.hook();

	PLH::x86Detour detour104((char*)0x0AEDC80, (char*)&HOOK_DisconnectSocket, &DisonnectSocket_old, dis);
	detour104.hook();

	PLH::x86Detour detour105((char*)0x0AEDD80, (char*)&HOOK_Idle, &Idle_old, dis);
	detour105.hook();

	PLH::x86Detour detour102((char*)0x08A19F0, (char*)&HOOK_GodfatherConnectionManager_CTOR, &GodfatherConnectionManager_CTOR_old, dis);
	detour102.hook();

	//PLH::x86Detour detour103((char*)0x0AEA650, (char*)&HOOK_HelloReceived, &HelloReceived_old, dis);
	//detour103.hook();
#endif // ENABLE_GF2_MULTIPLAYER

#if ENABLE_GF2_DISPL_BEGINSCENE_HOOK
	PLH::x86Detour detour102((char*)0x608620, (char*)&HOOK_Displ_BeginScene, &Displ_BeginScene_Old, dis);
	detour102.hook();
#endif // ENABLE_GF2_DISPL_BEGINSCENE_HOOK

	PLH::x86Detour detour170((char*)0x0403A50, (char*)&HOOK_StreamManager_Load, &StreamManager_Load_Old, dis);
	detour170.hook();

	PLH::x86Detour detour1720((char*)0x0403C40, (char*)&HOOK_StreamManager_LoadHierarchy, &StreamManager_LoadHierarchy_Old, dis);
	detour1720.hook();

	PLH::x86Detour detour171((char*)0x0448140, (char*)&HOOK_SimManager_LoadResource, &SimManager_LoadResource_Old, dis);
	detour171.hook();

	PLH::x86Detour detour155((char*)0x608930, (char*)&HOOK_Displ_EndScene, &Displ_EndScene_Old, dis);
	detour155.hook();

	PLH::x86Detour detour125((char*)0x608250, (char*)&HOOK_Displ_ResetDevice, &Displ_ResetDevice_Old, dis);
	detour125.hook();

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

	PLH::x86Detour detour1780((char*)0x0446340, (char*)&HOOK_SimManager_SpawnEntity, &SimManager_SpawnEntity_Old, dis);
	detour1780.hook();

	PLH::x86Detour detour1781((char*)0x8492F0, (char*)&HOOK_City_HandleAttributes, &City_HandleAttributes_Old, dis);
	detour1781.hook();
#endif // ENABLE_GF2_SPAWN_ENTITY_HOOKS

	PLH::x86Detour detour178((char*)0x06817C0, (char*)&Hook_OpenLevelServices, &OpenLevelServices_Old, dis);
	detour178.hook();

	PLH::x86Detour detour221((char*)0x0682860, (char*)&Hook_CloseLevelServices, &CloseLevelServices_Old, dis);
	detour221.hook();

	PLH::x86Detour detour1555((char*)0x09BCB30, (char*)&HOOK_MobfaceManager_SetTargetModel, &MobfaceManager_SetTargetModel_Old, dis);
	detour1555.hook();

	PLH::x86Detour detour1557((char*)0x0495010, (char*)&HOOK_EARSJobScheduler_GetDefaultThreadCount, &EARSJobScheduler_GetDefaultThreadCount_old, dis);
	detour1557.hook();

	PLH::x86Detour detour1558((char*)0x0841460, (char*)&HOOK_TrinityGameCamera_BeginUpdate, &TrinityGameCamera_BeginUpdate_Old, dis);
	detour1558.hook();

	EARS::Modules::DemographicRegion::StaticApplyHooks();
	EARS::Modules::ScoreKeeper::StaticApplyHooks();
	Mod::GammaFix::StaticApplyHooks();
	Mod::EdgeAA::StaticApplyHooks();
}

void GF2Hook::Init_GameSystems()
{
	ImGuiManager* OurImGuiManager = ImGuiManager::Get();
	OurImGuiManager->Open();

	DiscordManager* OurDiscordManager = DiscordManager::Get();
	OurDiscordManager->Open();

	Mod::ScripthookEvents_Open();
}

void GF2Hook::Tick()
{
	//static hook::Type<AptGlobal*> AptGlobalGlobal = hook::Type<AptGlobal*>(0x119CBB8);
	//static hook::Type<AptAnimationPoolData*> AptPool = hook::Type<AptAnimationPoolData*>(0x119A9CC);

	//if (AptGlobalGlobal)
	//{
	//	int z = 0;
	//}

	//if (AptPool)
	//{
	//	int z = 0;
	//}
}
