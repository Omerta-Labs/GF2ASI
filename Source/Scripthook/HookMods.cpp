#include "HookMods.h"

#include "Addons/Hook.h"

#include "SDK/EARS_Framework/Toolkits/StateMachine/SMBuilder.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerMasterSM.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDebugFlySM.h"

#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#define RUN_MASTER_SM_IN_ASI 1
#define IMPLEMENT_DEBUG_FLY_SM 1

EARS::StateMachineSys::StateMachine* S_PlayerMasterSM_FactoryFn(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
{
	EARS::Modules::PlayerMasterSM* MasterSM = new EARS::Modules::PlayerMasterSM(id, pSMParams);
	return MasterSM;
}

// PURPOSE: Ability to add custom state machines into the game at the same point of registration
//			as the rest of the state machines in the game.
uint64_t HOOK_BuildStateMachines_Old;
void _cdecl HOOK_BuildStateMachines()
{
	PLH::FnCast(HOOK_BuildStateMachines_Old, &HOOK_BuildStateMachines)();

	// register custom StateMachines after game engine versions
	EARS::Modules::PlayerDebugFlySM::BuildStateMachine();
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

	// TRANSITIONS TO DEBUG STATE ADDED FOR MOD
	StartState->AddTransition("debugFly", 0x11);
	StartState->AddExitMessage(0x31);

	// DEBUG STATE ADDED FOR MOD
	EARS::Framework::SMBuilderState* DebugFlyState = Builder.AddState("debugFly", -1);
	DebugFlyState->AddChild("playerDebugFlyStateTable", false);
	DebugFlyState->AddTransition("start", 1);

	Builder.CompileAndRegister(0xB08AE1F6, S_PlayerMasterSM_FactoryFn, "PlayerMasterSM");

	Builder.Destroy();
}

void Mod::ApplyHooks()
{
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

#if IMPLEMENT_DEBUG_FLY_SM
	PLH::x86Detour detour100((char*)0x067DEB0, (char*)&HOOK_BuildStateMachines, &HOOK_BuildStateMachines_Old, dis);
	detour100.hook();
#endif // IMPLEMENT_DEBUG_FLY_SM

#if RUN_MASTER_SM_IN_ASI
	PLH::x86Detour detour101((char*)0x07AAA00, (char*)&HOOK_PlayerMasterSM_BuildStateMachine, &HOOK_PlayerMasterSM_BuildStateMachine_Old, dis);
	detour101.hook();
#endif // RUN_MASTER_SM_IN_ASI
}
