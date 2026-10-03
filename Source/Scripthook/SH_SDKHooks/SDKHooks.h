#pragma once

//=============================================================================
// Hooks and start-up that act on SDK systems but belong to the modding layer.
//
// These lived inside the SDK files they target, which made Source/SDK depend on
// polyhook and on Scripthook/ScripthookEvents.h -- the bottom layer reaching up
// into the one above it. The detour bodies came with them, since a detour that
// dispatches a Scripthook event is not engine code by any reading.
//
// Each of these was a static member on an SDK class
// (ScoreKeeper::StaticApplyHooks, DemographicRegion::StaticApplyHooks) or a free
// function in an SDK namespace (EARS::Framework::InitialiseScripthookModLoader).
// They are free functions here, and the declarations are gone from the SDK
// headers.
//=============================================================================

namespace Mod::SDKHooks
{
	/**
	 * Detours ScoreKeeper::ExecuteOperation so unlock events reach
	 * Mod::DispatchPlatformAgnosticUnlockEvent.
	 */
	void ApplyScoreKeeperHooks();

	/**
	 * Detours the traffic and NPC instance limits. Compiled out unless
	 * DISABLE_NPC_SPAWN_LIMIT is set, in which case this does nothing.
	 */
	void ApplyDemographicRegionHooks();

	/**
	 * Scans simgroup_mods and mounts the behaviour overrides it finds. Must run
	 * after the attribute handlers are registered and before any stream loads.
	 */
	void InitialiseModLoader();
}
