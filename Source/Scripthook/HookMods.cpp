#include "HookMods.h"

// Addons
#include "Addons/Hook.h"

// Scripthook
#include "Scripthook/SH_PlayerMasterSM/PlayerAnimViewSM.h"
#include "Scripthook/SH_PlayerMasterSM/PlayerMasterSM_Modded.h"

// SDK
#include "SDK/EARS_Framework/Toolkits/StateMachine/SMBuilder.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerMasterSM.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDebugFlySM.h"
#include "SDK/EARS_Godfather/Modules/UI/UIPopup.h"
#include "SDK/EARS_Godfather/Modules/UI/UIFrontend.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/StateMachines/VehicleEntrySM.h"

#include "SDK/EARS_Godfather/Modules/Debug/DemoPackageManager.h"
#include "SDK/EARS_Godfather/Modules/Debug/DemoPackage.h"

// Pl2
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#define RUN_MASTER_SM_IN_ASI 1
#define IMPLEMENT_DEBUG_FLY_SM 0
#define IMPLEMENT_ANIM_VIEWER_SM (DEBUG && 0)
#define IMPLEMENT_PHOTO_MODE_SM (DEBUG && 0)
#define IMPLEMENT_DEMO_PACKAGE_POPUP (DEBUG && 0)
#define IMPLEMENT_VEHICLE_ENTRY_SM (DEBUG && 0)
#define OVERRIDE_LAUNCH_CMD (DEBUG && 0)

EARS::StateMachineSys::StateMachine* S_PlayerMasterSM_FactoryFn(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
{
	SH::PlayerMasterSM_Modded* MasterSM = new SH::PlayerMasterSM_Modded(id, pSMParams);
	return MasterSM;
}

// PURPOSE: Implement VehicleEntrySM in asi, run all code through our SDK
uint64_t HOOK_S_VehicleEntrySM_FactoryFn_Old;
EARS::StateMachineSys::StateMachine* S_VehicleEntrySM_FactoryFn(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
{
	return EARS::Modules::VehicleEntrySM::S_VehicleEntrySM_FactoryFn(id, pSMParams);
}

// PURPOSE: Ability to add custom state machines into the game at the same point of registration
//			as the rest of the state machines in the game.
uint64_t HOOK_BuildStateMachines_Old;
void _cdecl HOOK_BuildStateMachines()
{
	PLH::FnCast(HOOK_BuildStateMachines_Old, &HOOK_BuildStateMachines)();

	// register custom StateMachines after game engine versions
#if IMPLEMENT_DEBUG_FLY_SM
	EARS::Modules::PlayerDebugFlySM::BuildStateMachine();
#endif // IMPLEMENT_DEBUG_FLY_SM

#if IMPLEMENT_ANIM_VIEWER_SM
	SH::PlayerAnimViewSM::BuildStateMachine();
#endif // IMPLEMENT_ANIM_VIEWER_SM
}

// PURPOSE: Ability to extend the PlayerMasterSM with our own states, messages and transitions.
uint64_t HOOK_PlayerMasterSM_BuildStateMachine_Old;
void _cdecl HOOK_PlayerMasterSM_BuildStateMachine()
{
	//PLH::FnCast(HOOK_PlayerMasterSM_BuildStateMachine_Old, &HOOK_PlayerMasterSM_BuildStateMachine)();

	void* thread_new_allocator = MemUtils::CallCdeclMethod<void*>(0x09C8F80);
	EARS::Framework::SMBuilder Builder = EARS::Framework::SMBuilder::MakeBuilder("playerMasterStateTable", thread_new_allocator);
	EARS::Framework::SMBuilderState* StartState = Builder.AddState("start", -1);
	StartState->AddChild("playerLogicalStateTable", true);
	StartState->AddChild("playerLowerBodyStateTable", true);
	StartState->AddChild("playerUpperBodyStateTable", true);
	StartState->AddChild("playerCameraStateTable", true);
	StartState->AddChild("playerF2FStateTable", true);

	// Fly mode update
	StartState->AddUpdateMessage(0x900);

#if IMPLEMENT_DEBUG_FLY_SM

	// TRANSITIONS TO DEBUG STATE ADDED FOR MOD
	StartState->AddTransition("debugFly", 0x11);
	StartState->AddExitMessage(0x31);

	// DEBUG STATE ADDED FOR MOD
	EARS::Framework::SMBuilderState* DebugFlyState = Builder.AddState("debugFly", -1);
	DebugFlyState->AddChild("playerDebugFlyStateTable", false);
	DebugFlyState->AddTransition("start", 1);
#endif // IMPLEMENT_DEBUG_FLY_SM

#if IMPLEMENT_ANIM_VIEWER_SM
	// TRANSITIONS TO DEBUG STATE ADDED FOR MOD
	StartState->AddTransition("debugAnimView", 0x11);

	// DEBUG STATE ADDED FOR MOD
	EARS::Framework::SMBuilderState* DebugAnimViewState = Builder.AddState("debugAnimView", -1);
	DebugAnimViewState->AddChild("playerDebugAnimViewStateTable", false);
	DebugAnimViewState->AddChild("playerCameraStateTable", false);
	DebugAnimViewState->AddTransition("start", 2);
#endif // IMPLEMENT_ANIM_VIEWER_SM

	// TODO: While the scripthook features are defined in derived type, we still want to replace original PlayerMasterSM.
	Builder.CompileAndRegister(0xB08AE1F6, S_PlayerMasterSM_FactoryFn, "PlayerMasterSM");

	Builder.Destroy();
}

#if OVERRIDE_LAUNCH_CMD
// PURPOSE: Ability to extend the games init streams and spawn location
uint64_t UIFrontend_LaunchGame_Old;
typedef void (__thiscall* UIFrontend_LaunchGame)(void*, const char*, const char*, bool, const char*);
void _cdecl HOOK_UIFrontend_LaunchGame(const char* pMaps, const char* pStreams, bool bOnline, const char* pSpawn)
{
	static const char* ModifiedStreams = "cuba.str miami.str queens.str manhattan.str";
	PLH::FnCast(UIFrontend_LaunchGame_Old, &HOOK_UIFrontend_LaunchGame)(pMaps, ModifiedStreams, bOnline, pSpawn);
}
#endif // OVERRIDE_LAUNCH_CMD

#if IMPLEMENT_DEMO_PACKAGE_POPUP
void DemoPopupCallback(int SelectedOption, EARS::Apt::UIPopupInfo* PopupInfo, bool bAborted)
{
	if (bAborted)
	{

	}

	if (SelectedOption == -1)
	{

	}

	const int32_t SelectedValue = PopupInfo->GetUserValue(SelectedOption);
	if (SelectedValue == -4)
	{
		// show another
	}
	else
	{
		if (SelectedValue == -3)
		{
			hook::Type<int32_t> FrontendStateVal = hook::Type<int32_t>(0x00E5672C);
			FrontendStateVal.set(17);

			EARS::Apt::UIFrontend::GetInstance()->AddNextStateSequence(0);
		}
		else if (SelectedValue != -2)
		{
			EARS::Modules::DemoPackageManager* PackageMgr = EARS::Modules::DemoPackageManager::GetInstance();
			PackageMgr->SetPendingPackageByIndex(SelectedValue);

			// start new game
			EARS::Apt::UIFrontend::GetInstance()->StartNewGame();
		}
	}

	if (PopupInfo)
	{
		PopupInfo->~UIPopupInfo();
		MemUtils::CallCdeclMethod<void, void*>(0x9C8EB0, PopupInfo);
	}
}

static void EnterStateNew_NewGame(EARS::Apt::UIFrontend* _this, EARS::Apt::UIFrontend::FrontendState ePreviousState)
{
	int z = 0;
}

// PURPOSE: Ability to extend the games init streams and spawn location
uint64_t UIFrontend_EnterState_StartNewGame_Old;
typedef void(__thiscall* UIFrontend_EnterState_StartNewGame)(void*, int);
void __cdecl HOOK_UIFrontend_EnterState_StartNewGame(EARS::Apt::UIFrontend* _this, int ePreviousState)
{
	//UIFrontend_EnterState_StartNewGame funcCast = (UIFrontend_EnterState_StartNewGame)UIFrontend_EnterState_StartNewGame_Old;
	//funcCast(_this, ePreviousState);

	EARS::Apt::UIFrontend::PatchState((int)EARS::Apt::UIFrontend::FrontendState::START_NEW_GAME, EnterStateNew_NewGame);

	EARS::Modules::DemoPackageManager* PackageMgr = EARS::Modules::DemoPackageManager::GetInstance();

	// create popup
	void* alloc = MemUtils::CallCdeclMethod<uint8_t*>(0x09C8E50, sizeof(EARS::Apt::UIPopupInfo));
	EARS::Apt::UIPopupInfo* obj = new (alloc) EARS::Apt::UIPopupInfo();

	obj->SetStyle(EARS::Apt::UIPopupInfo::PopupStyle::STANDARD);
	obj->SetCallback(DemoPopupCallback);
	obj->SetDescription("Debug Options");
	obj->AddOption("New Game", 1, -2);
	obj->AddOption("Main Menu", 0, -3);

	bool bAddExtraPopup = false;
	uint32_t NumPackages = PackageMgr->GetNumPackages();
	if (NumPackages > 6)
	{
		NumPackages = 5;
		bAddExtraPopup = true;
	}

	for (uint32_t PackageIdx = 0; PackageIdx < NumPackages; PackageIdx++)
	{
		const EARS::Modules::DemoPackage* Package = PackageMgr->GetPackage(PackageIdx);
		assert(Package);

		const String* PackageName = Package->GetName();
		obj->AddOption(PackageName->c_str(), false, PackageIdx);
	}

	if (bAddExtraPopup)
	{
		obj->AddOption("More Demo Packages...", false, -4);
	}

	obj->Show();

}

uint64_t UIFrontend_ExecuteNextState_Old;
typedef void(__thiscall* UIFrontend_ExecuteNextState)(void*);
void __fastcall HOOK_UIFrontend_ExecuteNextState(EARS::Apt::UIFrontend* _this)
{
	//hook::Type<FrontendState> CurrentStateVal = hook::Type<FrontendState>(0x1130548);
	//hook::Type<FrontendState> NextStateVal = hook::Type<FrontendState>(0x0E5672C);
	hook::Type<Flags32> FrontendFlagsVal = hook::Type<Flags32>(0x01130590);

	EARS::Apt::UIFrontend::GetInstance()->ExecuteNextState();
}
#endif // IMPLEMENT_DEMO_PACKAGE_POPUP

void Mod::ApplyHooks()
{
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour100((char*)0x067DEB0, (char*)&HOOK_BuildStateMachines, &HOOK_BuildStateMachines_Old, dis);
	detour100.hook();

#if RUN_MASTER_SM_IN_ASI
	PLH::x86Detour detour101((char*)0x07AAA00, (char*)&HOOK_PlayerMasterSM_BuildStateMachine, &HOOK_PlayerMasterSM_BuildStateMachine_Old, dis);
	detour101.hook();
#endif // RUN_MASTER_SM_IN_ASI

#if IMPLEMENT_VEHICLE_ENTRY_SM
	PLH::x86Detour detour102((char*)0x08837A0, (char*)&S_VehicleEntrySM_FactoryFn, &HOOK_S_VehicleEntrySM_FactoryFn_Old, dis);
	detour102.hook();
#endif // IMPLEMENT_VEHICLE_ENTRY_SM

#if OVERRIDE_LAUNCH_CMD
	PLH::x86Detour detour105((char*)0x0930E70, (char*)&HOOK_UIFrontend_LaunchGame, &UIFrontend_LaunchGame_Old, dis);
	detour105.hook();
#endif // OVERRIDE_LAUNCH_CMD

#if IMPLEMENT_DEMO_PACKAGE_POPUP
	PLH::x86Detour detour106((char*)0x0931CF0, (char*)&HOOK_UIFrontend_EnterState_StartNewGame, &UIFrontend_EnterState_StartNewGame_Old, dis);
	detour106.hook();

	PLH::x86Detour detour107((char*)0x0931010, (char*)&HOOK_UIFrontend_ExecuteNextState, &UIFrontend_ExecuteNextState_Old, dis);
	detour107.hook();

	EARS::Apt::UIFrontend::CopyStates();
#endif // IMPLEMENT_DEMO_PACKAGE_POPUP
}
