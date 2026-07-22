#pragma once

// C++
#include <stdint.h>
#include <string>
#include <vector>

// SDK
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// forward declares
namespace EARS
{
	namespace Framework
	{
		class StreamManager;
	}
}

namespace SH
{
	/**
	 * Mirrors the engine's GFStreamAdditionHandler (PC sub_906360) so mod stream
	 * additions load as part of the game-database load lifecycle instead of at an
	 * arbitrary early point (which got them torn down when a savegame started loading).
	 *
	 * Linked to iMsgGameDatabaseLoading for the whole session. On each database load it
	 * arms itself for iMsgStreamBeginLoad; on the next stream begin-load it injects the
	 * additions once (via ModManager::ApplyStreamAdditions) and disarms. Re-arms on every
	 * subsequent database load, so additions are re-issued each time the DB reloads.
	 */
	class ModStreamAdditionHandler : public RWS::CEventHandler
	{
	public:

		void HandleEvents(const RWS::CMsg& Msg) override;

	private:

		bool m_bArmed = false;	// currently linked to iMsgStreamBeginLoad
	};
	/* One stream addition, parsed from an EA-format .ads descriptor */
	struct ModStreamAddition
	{
		std::string StreamName;
		uint32_t StreamGuid = 0;
	};

	/* One stream replacement (one line of a .rep descriptor) */
	struct ModStreamReplacement
	{
		std::string Path;			// relative TOC name of the stream being replaced (used to hash)
		uint32_t StreamGuid = 0;	// guid32 of the shipped stream to replace
		uint32_t NameHash = 0;		// SDBM(lowercase(<path without .str> + ".str")), computed at parse
		uint32_t Version = 1;		// conflict arbitration when two mods hit the same guid; highest wins
	};

	/* One stream belonging to a level addition (.add pair line) */
	struct ModLevelFile
	{
		std::string Path;		// path relative to the package prefix, e.g. "mp_cuba\terrain\medium.str"
		uint32_t StreamGuid = 0;	// zero = match by name hash only
		uint32_t NameHash = 0;	// SDBM(lowercase("<root>\<path>")), computed at parse time
	};

	/* One level addition, parsed from an EA-format .add descriptor (format version 4) */
	struct ModLevelAddition
	{
		std::string LevelName;	// line 3, downcased - frontend-facing level name
		std::string RootName;	// line 4, downcased - TOC root the streams hang under
		uint32_t RootHash = 0;	// SDBM(lowercase(RootName)), passed as TOCRootHash
		bool bFlag = false;		// line 2 ('1' = true; retail maps ship 0, meaning unconfirmed)
		std::vector<ModLevelFile> Files;
	};

	struct ModPackage
	{
		std::string Name;		// directory name under mods
		std::string Prefix;		// "mods\<Name>\" - registered via StreamManager::AddDevice
		std::vector<ModStreamAddition> Additions;
		std::vector<ModStreamReplacement> Replacements;
		std::vector<ModLevelAddition> Levels;
	};

	/**
	 * Scans mods\<package>\ directories for EA-format DLC descriptors and applies them
	 * through the game's own downloaded-content plumbing:
	 *   .rep -> StreamManager::MarkStreamFileForReplacement (highest version wins)
	 *   .ads -> StreamManager::Load with the package prefix, alongside the game database
	 * Loose file overrides need no descriptors - the "mods" search location handles those.
	 *
	 * Descriptor formats:
	 *   .ads: line 1 format version (must be 1), line 2 "0x<guid32>", line 3 stream filename
	 *   .rep: line 1 format version, then one replacement per line until EOF:
	 *         "<relative path> 0x<guid32> [version]". The path is the stream's TOC name; its
	 *         hash is what LoadHierarchy requires before it will redirect the file (a guid alone
	 *         only sets the downloaded flag). This mirrors how .add supplies a precomputed name
	 *         hash - the game itself never derives one from a bare guid.
	 *   .add: line 1 format version (must be 4), line 2 flag, line 3 level name, line 4 root name,
	 *         then "<path> 0x<guid32>" pairs until EOF. Applied as a batch of replacement records
	 *         (guid or name-hash matched, scoped to the root's TOC) so the level's own TOC stream
	 *         and everything beneath it resolve into the package directory when the game requests
	 *         the level by name. NOTE: frontend/level-list registration (GFLevelRecord) is not
	 *         implemented yet - the PC equivalent of GFLevelAdditionHandler still needs mapping.
	 */
	class ModManager
	{
	public:

		static ModManager& GetInstance();

		/* Discover mod packages on disk. Safe to call before the game singletons exist. */
		void ScanMods();

		/*
		 * Called around every StreamManager::Load (via the 0x403A50 detour).
		 * Begin applies replacements before the first load resolves and lazily links the
		 * stream-addition handler once iMsgGameDatabaseLoading is registered. End is a
		 * no-op now that additions are driven by the handler on iMsgStreamBeginLoad.
		 */
		void OnStreamManagerLoadBegin(EARS::Framework::StreamManager* StreamMgr);
		void OnStreamManagerLoadEnd(EARS::Framework::StreamManager* StreamMgr);

		/*
		 * Issues the actual StreamManager::Load calls for every parsed .ads addition.
		 * Driven by ModStreamAdditionHandler on iMsgStreamBeginLoad during a database
		 * load, so mod streams share that load's lifecycle. Re-entrancy guarded.
		 */
		void ApplyStreamAdditions();

		const std::vector<ModPackage>& GetPackages() const { return m_Packages; }

		/*
		 * The hash used by the stream system for name-based matching: classic SDBM
		 * (h = h * 65599 + c) with A-Z folded to lowercase, as done inline by
		 * StreamManager::UpdateTOCDownloadFlags when comparing TOC entry names.
		 */
		static uint32_t HashStreamName(const char* Name);

	private:

		void ScanPackageDescriptors(ModPackage& Package);
		bool ParseAdsFile(const char* FilePath, ModPackage& Package);
		bool ParseRepFile(const char* FilePath, ModPackage& Package);
		bool ParseAddFile(const char* FilePath, ModPackage& Package);

		/* Link m_pAdditionHandler to iMsgGameDatabaseLoading once that event exists. */
		void EnsureAdditionHandlerLinked();

		std::vector<ModPackage> m_Packages;
		bool m_bScanned = false;
		bool m_bReplacementsApplied = false;

		/* our own Load calls re-enter the same detour */
		bool m_bApplyingAdditions = false;

		/* stream-addition handler; heap-allocated once and intentionally never freed
		   so its ~CEventHandler never calls into a torn-down engine at process exit */
		ModStreamAdditionHandler* m_pAdditionHandler = nullptr;
		bool m_bHandlerLinked = false;
	};
}
