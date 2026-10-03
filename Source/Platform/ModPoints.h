#pragma once

//=============================================================================
// Points in the SDK where the modding layer may change what the engine does.
//
// Diagnostics.h and Host.h let the SDK *ask* the host something. This is the
// other direction: the host alters a decision the SDK is making. Same
// mechanism -- a function pointer installed at start-up, a null default that
// means "behave as the original did" -- but worth its own header, because the
// list will grow and this is the complete, auditable extension surface. Every
// point the SDK exposes is in this one file.
//
// Why not an RWS message for this:
//
//   - No substitution semantics. SendMsg is fire-and-forget to N handlers with
//     no return value, so "replace this value" needs an in/out payload and two
//     mods patching the same thing race with no defined winner. A resolver
//     returns one answer and the caller can see who provided it.
//   - Cost. These sit in streaming and per-entity paths. A null check plus an
//     indirect call is cheaper than a hash lookup and a handler walk.
//   - No type checking. A message is an SDBM hash of a name, so nothing
//     verifies a handler's assumed payload against the emitter's. A function
//     pointer gives a compile error instead of a bad cast.
//
// Messages remain right for notifications -- several listeners, nothing to
// change -- which is what Scripthook/ScripthookEvents.h is for.
//
// Adding a point means editing the SDK at the call site. There is no way round
// that: an injection point *is* a call site, and it has to live in the code
// being injected into. What this buys is that each one costs one line there and
// mentions nothing from the modding layer. For anything not exposed here, the
// escape hatch is a polyhook detour on the PC address, which is what
// Scripthook/HookMods.cpp already does.
//
// Threading: install during start-up, before the systems that read these run.
// Nothing here is synchronised, because nothing is expected to change after
// start-up.
//=============================================================================

namespace EARS::Common
{
	struct guid128_t;
}

namespace RWS
{
	class CAttributePacket;
}

namespace EARS::ModPoints
{
	//-------------------------------------------------------------------------
	// SimManager::LoadResource -- substitute an attribute packet while a sim
	// group's packet array is being fixed up.
	//
	// Called once per packet, after pointer recovery and before the sim group
	// is committed to a list, which is why it cannot be done from a detour on
	// LoadResource without reimplementing the function. Return nullptr to keep
	// the original packet.
	//
	// The mod loader uses this to apply the .simgroup files under
	// simgroup_mods: it owns the parsed overrides and answers by packet guid.
	//-------------------------------------------------------------------------
	using ResolveAttributePacketFn =
		RWS::CAttributePacket* (*)(const EARS::Common::guid128_t& PacketId);

	void SetResolveAttributePacket(ResolveAttributePacketFn Fn);

	/** nullptr when no resolver is installed, or when it declines this packet. */
	RWS::CAttributePacket* ResolveAttributePacket(const EARS::Common::guid128_t& PacketId);
}
