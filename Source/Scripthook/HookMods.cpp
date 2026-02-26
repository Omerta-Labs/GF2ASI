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

// Pl2
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#define RUN_MASTER_SM_IN_ASI 1
#define IMPLEMENT_DEBUG_FLY_SM 0
#define IMPLEMENT_ANIM_VIEWER_SM 0
#define IMPLEMENT_PHOTO_MODE_SM 0
#define OVERRIDE_LAUNCH_CMD 0

EARS::StateMachineSys::StateMachine* S_PlayerMasterSM_FactoryFn(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
{
	SH::PlayerMasterSM_Modded* MasterSM = new SH::PlayerMasterSM_Modded(id, pSMParams);
	return MasterSM;
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

void Mod::ApplyHooks()
{
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour100((char*)0x067DEB0, (char*)&HOOK_BuildStateMachines, &HOOK_BuildStateMachines_Old, dis);
	detour100.hook();

#if RUN_MASTER_SM_IN_ASI
	PLH::x86Detour detour101((char*)0x07AAA00, (char*)&HOOK_PlayerMasterSM_BuildStateMachine, &HOOK_PlayerMasterSM_BuildStateMachine_Old, dis);
	detour101.hook();
#endif // RUN_MASTER_SM_IN_ASI

#if OVERRIDE_LAUNCH_CMD
	PLH::x86Detour detour105((char*)0x0930E70, (char*)&HOOK_UIFrontend_LaunchGame, &UIFrontend_LaunchGame_Old, dis);
	detour105.hook();
#endif // OVERRIDE_LAUNCH_CMD
}
