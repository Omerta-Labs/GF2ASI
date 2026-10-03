#pragma once

//=============================================================================
// Detours and start-up that act on SDK systems.
//
// All of these run from GF2Hook during start-up. Order matters where noted;
// otherwise they are independent.
//=============================================================================

namespace Mod::SDKHooks
{
	/**
	 * Detours ScoreKeeper::ExecuteOperation so score operations that unlock
	 * something are forwarded to the Scripthook's platform-agnostic unlock
	 * path. Only operations the original accepted are forwarded.
	 */
	void ApplyScoreKeeperHooks();

	/**
	 * Detours the parked-car, pedestrian and vehicle traffic managers' instance
	 * limits so they always report headroom. Compiled out unless
	 * DISABLE_NPC_SPAWN_LIMIT is set.
	 */
	void ApplyDemographicRegionHooks();

	/**
	 * Scans simgroup_mods and mounts the behaviour overrides it finds. Must run
	 * after the attribute handlers are registered and before any stream loads.
	 */
	void InitialiseModLoader();
}
