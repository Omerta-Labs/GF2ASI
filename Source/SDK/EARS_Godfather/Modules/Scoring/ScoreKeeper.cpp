#include "ScoreKeeper.h"

// GF2
#include "SDK/EARS_Godfather/Modules/Scoring/ScoreEvent.h"

// Renderware
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Addons
#include "Addons/Hook.h"
#include "Scripthook/ScripthookEvents.h"

// Polyhook
#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

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

void EARS::Modules::ScoreKeeper::StaticApplyHooks()
{
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour151((char*)0x08948A0, (char*)&EARS::Modules::HOOK_ScoreKeeper_ExecuteOperation, &EARS::Modules::ScoreKeeper_ExecuteOperation_Old, dis);
	detour151.hook();
}
