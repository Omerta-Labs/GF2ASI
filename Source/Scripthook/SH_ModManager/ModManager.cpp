#include "ModManager.h"

// Addons
#include "Addons/tConsole.h"

// SDK
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"

// C++
#include <stdio.h>
#include <Windows.h>

namespace
{
	const char* MODS_ROOT = "mods";

	// Priority handed to Load for .ads stream additions. GFStreamAdditionHandler
	// forwards the priority of the stream whose BeginLoad triggered it; a fixed
	// default keeps mod streams behind whatever the game asked for first.
	const float MOD_STREAM_PRIORITY = 1.0f;

	// Engine CEventId globals (godfather2.exe) the addition handler links to, and the
	// priority GFStreamAdditionHandler uses for its stream-message links.
	RWS::CEventId* const iMsgGameDatabaseLoading = reinterpret_cast<RWS::CEventId*>(0x1206A28);
	RWS::CEventId* const iMsgStreamBeginLoad = reinterpret_cast<RWS::CEventId*>(0x1206794);
	const uint16_t STREAM_MSG_PRIORITY = 0x8000;

	std::string StripExtension(const std::string& FileName)
	{
		const size_t DotPos = FileName.find_last_of('.');
		const size_t SlashPos = FileName.find_last_of("\\/");
		if (DotPos != std::string::npos && (SlashPos == std::string::npos || DotPos > SlashPos))
		{
			return FileName.substr(0, DotPos);
		}

		return FileName;
	}

	std::string ToLower(const std::string& Input)
	{
		std::string Result = Input;
		for (char& Ch : Result)
		{
			if (Ch >= 'A' && Ch <= 'Z')
			{
				Ch += 32;
			}
		}

		return Result;
	}

	bool ReadLine(FILE* File, char* Buffer, size_t BufferSize)
	{
		if (!fgets(Buffer, static_cast<int>(BufferSize), File))
		{
			return false;
		}

		// strip trailing newline / carriage return
		for (char* Ch = Buffer; *Ch; ++Ch)
		{
			if (*Ch == '\r' || *Ch == '\n')
			{
				*Ch = 0;
				break;
			}
		}

		return true;
	}
}

SH::ModManager& SH::ModManager::GetInstance()
{
	static SH::ModManager s_Instance;
	return s_Instance;
}

void SH::ModManager::ScanMods()
{
	if (m_bScanned)
	{
		return;
	}
	m_bScanned = true;

	char SearchPath[MAX_PATH];
	sprintf_s(SearchPath, "%s\\*", MODS_ROOT);

	WIN32_FIND_DATAA FindData;
	HANDLE FindHandle = FindFirstFileA(SearchPath, &FindData);
	if (FindHandle == INVALID_HANDLE_VALUE)
	{
		tConsole::fPrintf("ModManager: no '%s' directory found, skipping scan", MODS_ROOT);
		return;
	}

	do
	{
		if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			continue;
		}

		if (FindData.cFileName[0] == '.')
		{
			continue;
		}

		ModPackage Package;
		Package.Name = FindData.cFileName;
		Package.Prefix = std::string(MODS_ROOT) + "\\" + Package.Name + "\\";
		ScanPackageDescriptors(Package);

		if (!Package.Additions.empty() || !Package.Replacements.empty() || !Package.Levels.empty())
		{
			tConsole::fPrintf("ModManager: package '%s' (%u additions, %u replacements, %u levels)",
				Package.Name.c_str(), Package.Additions.size(), Package.Replacements.size(), Package.Levels.size());
			m_Packages.push_back(Package);
		}
	} while (FindNextFileA(FindHandle, &FindData));

	FindClose(FindHandle);

	tConsole::fPrintf("ModManager: %u stream package(s) discovered", m_Packages.size());
}

uint32_t SH::ModManager::HashStreamName(const char* Name)
{
	uint32_t Hash = 0;
	for (const char* Ch = Name; *Ch; ++Ch)
	{
		uint32_t Value = static_cast<uint8_t>(*Ch);
		if (Value >= 'A' && Value <= 'Z')
		{
			Value += 32;
		}

		Hash = (Hash * 65599) + Value;
	}

	return Hash;
}

void SH::ModManager::ScanPackageDescriptors(ModPackage& Package)
{
	const char* DescriptorTypes[] = { "ads", "rep", "add" };
	for (const char* Type : DescriptorTypes)
	{
		char SearchPath[MAX_PATH];
		sprintf_s(SearchPath, "%s\\%s\\*.%s", MODS_ROOT, Package.Name.c_str(), Type);

		WIN32_FIND_DATAA FindData;
		HANDLE FindHandle = FindFirstFileA(SearchPath, &FindData);
		if (FindHandle == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		do
		{
			char FilePath[MAX_PATH];
			sprintf_s(FilePath, "%s\\%s\\%s", MODS_ROOT, Package.Name.c_str(), FindData.cFileName);

			bool bParsed = false;
			if (strcmp(Type, "ads") == 0)
			{
				bParsed = ParseAdsFile(FilePath, Package);
			}
			else if (strcmp(Type, "rep") == 0)
			{
				bParsed = ParseRepFile(FilePath, Package);
			}
			else
			{
				bParsed = ParseAddFile(FilePath, Package);
			}

			if (!bParsed)
			{
				tConsole::fPrintf("ModManager: failed to parse '%s'", FilePath);
			}
		} while (FindNextFileA(FindHandle, &FindData));

		FindClose(FindHandle);
	}
}

bool SH::ModManager::ParseAdsFile(const char* FilePath, ModPackage& Package)
{
	FILE* File = nullptr;
	if (fopen_s(&File, FilePath, "r") != 0 || !File)
	{
		return false;
	}

	bool bResult = false;
	char Line[MAX_PATH];

	uint32_t FormatVersion = 0;
	uint32_t StreamGuid = 0;
	if (ReadLine(File, Line, sizeof(Line)) && sscanf_s(Line, "%u", &FormatVersion) == 1 && FormatVersion == 1
		&& ReadLine(File, Line, sizeof(Line)) && sscanf_s(Line, "0x%x", &StreamGuid) == 1
		&& ReadLine(File, Line, sizeof(Line)) && Line[0])
	{
		ModStreamAddition Addition;
		Addition.StreamName = Line;
		Addition.StreamGuid = StreamGuid;
		Package.Additions.push_back(Addition);
		bResult = true;
	}

	fclose(File);
	return bResult;
}

bool SH::ModManager::ParseRepFile(const char* FilePath, ModPackage& Package)
{
	FILE* File = nullptr;
	if (fopen_s(&File, FilePath, "r") != 0 || !File)
	{
		return false;
	}

	char Line[MAX_PATH * 2];

	// line 1: format version
	uint32_t FormatVersion = 0;
	if (!ReadLine(File, Line, sizeof(Line)) || sscanf_s(Line, "%u", &FormatVersion) != 1)
	{
		fclose(File);
		return false;
	}

	// remaining lines: "<relative path> 0x<guid> [version]", one replacement each, until EOF
	size_t Added = 0;
	while (ReadLine(File, Line, sizeof(Line)))
	{
		if (Line[0] == 0)
		{
			continue;
		}

		char Path[MAX_PATH];
		uint32_t StreamGuid = 0;
		uint32_t Version = 1;

		const int Fields = sscanf_s(Line, "%s 0x%x %u",
			Path, static_cast<unsigned int>(sizeof(Path)), &StreamGuid, &Version);
		if (Fields < 2)
		{
			// not a "<path> 0x<guid>" pair - skip stray/comment lines quietly
			continue;
		}

		ModStreamReplacement Replacement;
		Replacement.Path = Path;
		Replacement.StreamGuid = StreamGuid;
		Replacement.Version = (Fields >= 3) ? Version : 1;

		// hash exactly what LoadHierarchy hashes: it strips any ".str" off the TOC name
		// then re-appends it before hashing, so normalise the same way here
		Replacement.NameHash = HashStreamName((StripExtension(Replacement.Path) + ".str").c_str());

		Package.Replacements.push_back(Replacement);
		++Added;
	}

	fclose(File);
	return Added > 0;
}

bool SH::ModManager::ParseAddFile(const char* FilePath, ModPackage& Package)
{
	FILE* File = nullptr;
	if (fopen_s(&File, FilePath, "r") != 0 || !File)
	{
		return false;
	}

	bool bResult = false;
	char Line[MAX_PATH * 2];

	uint32_t FormatVersion = 0;
	if (ReadLine(File, Line, sizeof(Line)) && sscanf_s(Line, "%u", &FormatVersion) == 1 && FormatVersion == 4)
	{
		ModLevelAddition Level;

		if (ReadLine(File, Line, sizeof(Line)))
		{
			Level.bFlag = (Line[0] == '1');
		}

		if (ReadLine(File, Line, sizeof(Line)) && Line[0])
		{
			Level.LevelName = ToLower(Line);
		}

		if (ReadLine(File, Line, sizeof(Line)) && Line[0])
		{
			Level.RootName = ToLower(Line);
			Level.RootHash = HashStreamName(Level.RootName.c_str());
		}

		if (!Level.LevelName.empty() && !Level.RootName.empty())
		{
			// remaining lines: "<path> 0x<guid32>" pairs until EOF
			while (ReadLine(File, Line, sizeof(Line)))
			{
				char Path[MAX_PATH];
				uint32_t StreamGuid = 0;
				if (sscanf_s(Line, "%s 0x%x", Path, static_cast<unsigned int>(sizeof(Path)), &StreamGuid) != 2)
				{
					continue;
				}

				ModLevelFile LevelFile;
				LevelFile.Path = Path;
				LevelFile.StreamGuid = StreamGuid;

				// the engine matches TOC entries against SDBM(lowercase("<root>\<path>"))
				const std::string HashInput = Level.RootName + "\\" + LevelFile.Path;
				LevelFile.NameHash = HashStreamName(HashInput.c_str());

				Level.Files.push_back(LevelFile);
			}

			if (!Level.Files.empty())
			{
				Package.Levels.push_back(Level);
				bResult = true;
			}
		}
	}

	fclose(File);
	return bResult;
}

void SH::ModManager::OnStreamManagerLoadBegin(EARS::Framework::StreamManager* StreamMgr)
{
	if (!StreamMgr || m_Packages.empty())
	{
		return;
	}

	// Link the addition handler as soon as iMsgGameDatabaseLoading is registered
	// (retried on each Load until the DatabaseFileManager ctor has run).
	EnsureAdditionHandlerLinked();

	if (m_bReplacementsApplied)
	{
		return;
	}
	m_bReplacementsApplied = true;

	for (const ModPackage& Package : m_Packages)
	{
		// the prefix must be registered before any replacement referencing it:
		// retail silently maps unknown prefixes to index 0
		StreamMgr->AddDevice(Package.Prefix.c_str());

		const String PrefixStr(Package.Prefix.c_str());
		for (const ModStreamReplacement& Replacement : Package.Replacements)
		{
			// The record carries both halves LoadHierarchy demands: the guid (sets the
			// downloaded flag when the TOC registers) and the precomputed name hash
			// (gates the actual path redirect). TOCRootHash 0 = any TOC.
			StreamMgr->MarkStreamFileForReplacement(
				EARS::Common::guid32_t(Replacement.StreamGuid), PrefixStr,
				Replacement.NameHash, 0, Replacement.Version);

			tConsole::fPrintf("ModManager: replace stream 0x%x ('%s', namehash 0x%x) -> '%s' (version %u)",
				Replacement.StreamGuid, Replacement.Path.c_str(), Replacement.NameHash,
				Package.Prefix.c_str(), Replacement.Version);
		}

		// Level additions: pre-arm a replacement record per stream so the level's
		// TOC stream and everything beneath it resolve into the package directory
		// once the game requests the level by name (mirrors LevelAdditionHandler::
		// EnableResource). Frontend registration is a separate, still-missing step.
		for (const ModLevelAddition& Level : Package.Levels)
		{
			for (const ModLevelFile& LevelFile : Level.Files)
			{
				StreamMgr->MarkStreamFileForReplacement(
					EARS::Common::guid32_t(LevelFile.StreamGuid), PrefixStr,
					LevelFile.NameHash, Level.RootHash, 1);
			}

			tConsole::fPrintf("ModManager: level '%s' armed (%u streams under root '%s')",
				Level.LevelName.c_str(), Level.Files.size(), Level.RootName.c_str());
		}
	}
}

void SH::ModManager::OnStreamManagerLoadEnd(EARS::Framework::StreamManager* StreamMgr)
{
	// Stream additions used to be injected here (right after the first Load), which put
	// them outside the game-database load lifecycle - so a savegame load tore them down.
	// They are now issued by ModStreamAdditionHandler on iMsgStreamBeginLoad instead.
	// Nothing to do post-Load.
	(void)StreamMgr;
}

void SH::ModManager::EnsureAdditionHandlerLinked()
{
	if (m_bHandlerLinked || m_Packages.empty())
	{
		return;
	}

	// iMsgGameDatabaseLoading is registered by the DatabaseFileManager ctor; until then
	// its id is 0 and LinkMsg would bind to the wrong (empty) message - so wait.
	if (iMsgGameDatabaseLoading->GetMsgId() == 0)
	{
		return;
	}

	if (!m_pAdditionHandler)
	{
		m_pAdditionHandler = new ModStreamAdditionHandler();
	}

	m_pAdditionHandler->LinkMsg(iMsgGameDatabaseLoading, STREAM_MSG_PRIORITY);
	m_bHandlerLinked = true;

	tConsole::fPrintf("ModManager: stream-addition handler linked to iMsgGameDatabaseLoading");
}

void SH::ModManager::ApplyStreamAdditions()
{
	if (m_bApplyingAdditions || m_Packages.empty())
	{
		return;
	}

	EARS::Framework::StreamManager* StreamMgr = EARS::Framework::StreamManager::GetInstance();
	if (!StreamMgr)
	{
		return;
	}
	m_bApplyingAdditions = true;

	for (const ModPackage& Package : m_Packages)
	{
		for (const ModStreamAddition& Addition : Package.Additions)
		{
			// Load strips the extension and re-appends ".str" itself, but keep the
			// name clean so handle lookups by name behave predictably
			const std::string StreamName = StripExtension(Addition.StreamName);
			const EARS::Common::guid32_t StreamGuid(Addition.StreamGuid);

			const uint32_t Handle = StreamMgr->Load(
				StreamName.c_str(), MOD_STREAM_PRIORITY,
				EARS::Framework::STREAM_LOADFLAG_DOWNLOADED, &StreamGuid, Package.Prefix.c_str());

			tConsole::fPrintf("ModManager: added stream '%s%s.str' (0x%x) -> handle %u",
				Package.Prefix.c_str(), StreamName.c_str(), Addition.StreamGuid, Handle);
		}
	}

	m_bApplyingAdditions = false;
}

void SH::ModStreamAdditionHandler::HandleEvents(const RWS::CMsg& Msg)
{
	const uint32_t EventId = Msg.GetEventID();

	if (EventId == iMsgGameDatabaseLoading->GetMsgId())
	{
		// A database load has begun - arm for its first stream begin-load.
		if (!m_bArmed)
		{
			LinkMsg(iMsgStreamBeginLoad, STREAM_MSG_PRIORITY);
			m_bArmed = true;
		}
	}
	else if (EventId == iMsgStreamBeginLoad->GetMsgId())
	{
		// Unlink FIRST: the Load calls below dispatch iMsgStreamBeginLoad themselves, so
		// this keeps us from re-entering. Then inject the additions once for this DB load.
		UnlinkMsg(iMsgStreamBeginLoad);
		m_bArmed = false;

		SH::ModManager::GetInstance().ApplyStreamAdditions();
	}
}
