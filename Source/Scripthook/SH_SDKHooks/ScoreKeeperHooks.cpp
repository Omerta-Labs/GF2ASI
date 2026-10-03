#include "Scripthook/SH_SDKHooks/SDKHooks.h"

// SDK
#include "modules/scoring/scorekeeper.h"
#include "modules/scoring/scoreevent.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "Platform/MemUtils.h"
#include "Scripthook/ScripthookEvents.h"

#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

// Moved out of ears_godfather/src/modules/scoring/scorekeeper.cpp, which held
// nothing else: the detour calls Mod::DispatchPlatformAgnosticUnlockEvent, so
// the whole file was modding-layer code living under Source/SDK. The .cpp is
// gone; scorekeeper.h keeps the reconstructed ScoreKeeper class.
// HOOKING AND SCRIPTHOOK RELATED
namespace EARS
{
	namespace Modules
	{
		uint64_t ScoreKeeper_ExecuteOperation_Old;
		typedef bool(__thiscall* ScoreKeeper_ExecuteOperation)(EARS::Modules::ScoreKeeper*, const EARS::Modules::ScoreEventOperation*, void*);
		bool __fastcall HOOK_ScoreKeeper_ExecuteOperation(EARS::Modules::ScoreKeeper* pThis, void* ecx, const EARS::Modules::ScoreEventOperation* a1, void* a2)
		{
			ScoreKeeper_ExecuteOperation funcCast = (ScoreKeeper_ExecuteOperation)ScoreKeeper_ExecuteOperation_Old;
			if (funcCast(pThis, a1, a2))
			{
				Mod::DispatchPlatformAgnosticUnlockEvent(*a1);
				return true;
			}

			return false;
		}
	}
}

void Mod::SDKHooks::ApplyScoreKeeperHooks()
{
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour151((char*)0x08948A0, (char*)&EARS::Modules::HOOK_ScoreKeeper_ExecuteOperation, &EARS::Modules::ScoreKeeper_ExecuteOperation_Old, dis);
	detour151.hook();
}
