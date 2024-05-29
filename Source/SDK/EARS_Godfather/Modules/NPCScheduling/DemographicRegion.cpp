#include "DemographicRegion.h"

#include <polyhook2/Detour/x86Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

#define DISABLE_NPC_SPAWN_LIMIT 0

#if DISABLE_NPC_SPAWN_LIMIT
uint64_t ParkedCarTrafficManager_IsBelowInstanceLimit_Old;
typedef bool(__thiscall* TrafficManager_IsBelowInstanceLimit)(void* pThis);
bool __fastcall HOOK_ParkedCarTrafficManager_IsBelowInstanceLimit(void* pThis, void* ecx)
{
	TrafficManager_IsBelowInstanceLimit funcCast = (TrafficManager_IsBelowInstanceLimit)ParkedCarTrafficManager_IsBelowInstanceLimit_Old;
	//return funcCast(pThis);
	return true;
}

uint64_t PedTrafficManager_IsBelowInstanceLimit_Old;
bool __fastcall HOOK_PedTrafficManager_IsBelowInstanceLimit(void* pThis, void* ecx)
{
	TrafficManager_IsBelowInstanceLimit funcCast = (TrafficManager_IsBelowInstanceLimit)PedTrafficManager_IsBelowInstanceLimit_Old;
	//return funcCast(pThis);
	return true;
}

uint64_t VehicleManager_IsBelowInstanceLimit_Old;
bool __fastcall HOOK_VehicleTrafficManager_IsBelowInstanceLimit(void* pThis, void* ecx)
{
	TrafficManager_IsBelowInstanceLimit funcCast = (TrafficManager_IsBelowInstanceLimit)VehicleManager_IsBelowInstanceLimit_Old;
	//return funcCast(pThis);
	return true;
}

/**
 * Hook to allow our systems to receive a tick
 * Good example is ImGui, so we can trigger bespoke behaviour.
 * Unfortunately we cannot replicate their approach (a class with an RWS::CEventHandler)
 */
uint64_t HandleEntry_Old;
typedef void(__thiscall* HandleEntry)(EARS::Modules::DemographicRegion* pThis);
void __fastcall HOOK_HandleEntry(EARS::Modules::DemographicRegion* pThis, void* ecx)
{
	pThis->ApplyUserSettings();

	HandleEntry funcCast = (HandleEntry)HandleEntry_Old;
	funcCast(pThis);
}
#endif // DISABLE_NPC_SPAWN_LIMIT

void EARS::Modules::DemographicRegion::ApplyUserSettings()
{
	// NPCs
	m_NPCFilters[0].m_MaxCount = 500;
	m_NPCFilters[1].m_MaxCount = 500;
	m_NPCFilters[2].m_MaxCount = 500;

	// Cars
	m_VehicleFilters[0].m_MaxParkedCount = 500;
	m_VehicleFilters[1].m_MaxParkedCount = 500;
	m_VehicleFilters[2].m_MaxParkedCount = 500;
	m_VehicleFilters[0].m_MaxCivilianCount = 500;
	m_VehicleFilters[1].m_MaxCivilianCount = 500;
	m_VehicleFilters[2].m_MaxCivilianCount = 500;
}

void EARS::Modules::DemographicRegion::StaticApplyHooks()
{
#if DISABLE_NPC_SPAWN_LIMIT
	PLH::ZydisDisassembler dis(PLH::Mode::x86);

	PLH::x86Detour detour151((char*)0x083AC70, (char*)&HOOK_ParkedCarTrafficManager_IsBelowInstanceLimit, &ParkedCarTrafficManager_IsBelowInstanceLimit_Old, dis);
	detour151.hook();

	PLH::x86Detour detour152((char*)0x083AFE0, (char*)&HOOK_PedTrafficManager_IsBelowInstanceLimit, &PedTrafficManager_IsBelowInstanceLimit_Old, dis);
	detour152.hook();

	PLH::x86Detour detour153((char*)0x083F4C0, (char*)&HOOK_VehicleTrafficManager_IsBelowInstanceLimit, &VehicleManager_IsBelowInstanceLimit_Old, dis);
	detour153.hook();

	PLH::x86Detour detour155((char*)0x078D700, (char*)&HOOK_HandleEntry, &HandleEntry_Old, dis);
	detour155.hook();
#endif // DISABLE_NPC_SPAWN_LIMIT
}
