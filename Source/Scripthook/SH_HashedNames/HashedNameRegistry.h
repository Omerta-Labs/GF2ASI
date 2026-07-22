#pragma once

// C++
#include <cstdint>
#include <string>
#include <unordered_map>

namespace SH
{
	/**
	 * Recovers human-readable names for SDBM string hashes seen at runtime.
	 *
	 * The engine hashes names (case-insensitive SDBM) and discards the string, so a
	 * hash cannot be reversed on its own. This registry keeps a side table (alive for
	 * the process lifetime) so hashes can be resolved back to names for debugging and
	 * tooling. It is general-purpose: every string routed through the HashString_SDBM
	 * hook flows in here, not just event ids.
	 *
	 * Populated by the HashString_SDBM / RegisterMsg detours in HookMods.cpp. Not
	 * thread-safe by design (mirrors the other SH singletons); the hooked paths run on
	 * the main/streaming flow.
	 */
	class HashedNameRegistry
	{
	public:

		static HashedNameRegistry& GetInstance();

		// Record Hash -> Name. The first non-empty name seen for a hash wins; a null or
		// empty name records nothing. Cheap for repeat hashes (no allocation once known).
		void Register(uint32_t Hash, const char* Name);

		// Convenience: hash Name (case-insensitive SDBM) and record it. Returns the hash.
		uint32_t RegisterName(const char* Name);

		// Flag a hash as a registered RWS message (event id). The name, if any, comes
		// from the HashString_SDBM hook that ran just before registration.
		void MarkAsMessage(uint32_t Hash);

		// Reverse lookup. Returns nullptr if the hash was never seen.
		const char* Find(uint32_t Hash) const;

		bool IsKnown(uint32_t Hash) const;
		bool IsMessage(uint32_t Hash) const;

	private:

		HashedNameRegistry() = default;

		struct Entry
		{
			std::string Name;
			bool bRegisteredMessage = false;
		};

		std::unordered_map<uint32_t, Entry> m_Names;
	};
}
