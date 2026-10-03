#pragma once

//=============================================================================
// Points at which a host may change an engine decision.
//
// Each point is a function pointer the host installs at start-up. With none
// installed the SDK behaves as the original game did, so a call site is always
// safe to add.
//
// This header is the complete set. Resolvers return a single answer rather than
// notifying a list; where several listeners and no return value is the right
// shape, use an RWS message instead (Scripthook/ScripthookEvents.h).
//
// Nothing here is synchronised. Install during start-up, before the systems
// that read these begin running.
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
	// Substitutes an attribute packet while a sim group's packet array is being
	// fixed up.
	//
	// Called from SimManager::LoadResource once per packet, after pointer
	// recovery and before the sim group is committed to a list. Return nullptr
	// to keep the original packet.
	//-------------------------------------------------------------------------
	using ResolveAttributePacketFn =
		RWS::CAttributePacket* (*)(const EARS::Common::guid128_t& PacketId);

	void SetResolveAttributePacket(ResolveAttributePacketFn Fn);

	/** nullptr when no resolver is installed, or when it declines this packet. */
	RWS::CAttributePacket* ResolveAttributePacket(const EARS::Common::guid128_t& PacketId);
}
