#include "Scripthook/SH_SDKHooks/SDKHooks.h"

// SDK
#include "framework/core/simmanager/simmanager.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/streammanager/streammanager.h"

#include "ears_common/recoverptr.h"

#include "Platform/Diagnostics.h"
#include "Platform/MemUtils.h"
#include "Platform/ModPoints.h"

#include <filesystem>
#include <fstream>
#include <map>
#include <vector>

namespace
{
	// Parsed .sgp files, owned here and freed at exit.
	std::vector<EARS::Framework::SimGroupTOC*> g_LoadedOverrideFiles;

	// Packet guid -> replacement, pointing into one of the TOCs above.
	std::map<EARS::Common::guid128_t, RWS::CAttributePacket*> g_RegisteredPackets;

	RWS::CAttributePacket* ResolveOverridePacket(const EARS::Common::guid128_t& PacketId)
	{
		const auto Found = g_RegisteredPackets.find(PacketId);
		return Found != g_RegisteredPackets.end() ? Found->second : nullptr;
	}

	void DestroyTOC()
	{
		for (EARS::Framework::SimGroupTOC* SimGroup : g_LoadedOverrideFiles)
		{
			delete SimGroup;
		}

		g_LoadedOverrideFiles.clear();
		g_RegisteredPackets.clear();
	}
}

void Mod::SDKHooks::InitialiseModLoader()
{
	static const std::filesystem::path MODS_FOLDER_NAME = "simgroup_mods";

	// use primitive module file name because otherwise std::filesystem produces bad results
	// for example double'd up scripts folder in path.
	wchar_t RawExeBuffer[256];
	memset(RawExeBuffer, 0, 256);
	GetModuleFileNameW(nullptr, RawExeBuffer, 256);

	// TODO: Could probably move this to utility header
	const std::filesystem::path ExecutablePath = RawExeBuffer;
	const std::filesystem::path CompletePath = (ExecutablePath.parent_path() / MODS_FOLDER_NAME);
	if (!std::filesystem::exists(CompletePath))
	{
		EARS::Diag::Printf("ERROR: SimGroupOverride path [%s] does not exist!", CompletePath.string().data());
		return;
	}

	for (const auto& dirEntry : std::filesystem::directory_iterator(CompletePath))
	{
		if (!dirEntry.is_regular_file())
		{
			continue;
		}

		const std::filesystem::path& AsPath = dirEntry.path();
		if (AsPath.has_extension() && AsPath.extension() == ".sgp")
		{
			EARS::Diag::Printf("Detected SimGroupOverride file [%s]", AsPath.c_str());

			std::ifstream input(AsPath.c_str(), std::ios::binary);
			const std::vector<char> Bytes((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
			input.close();

			uint8_t* SimGroupBytes = new uint8_t[Bytes.size()];
			memcpy(SimGroupBytes, Bytes.data(), Bytes.size());

			EARS::Framework::SimGroupTOC* SimGroupTOC = reinterpret_cast<EARS::Framework::SimGroupTOC*>(SimGroupBytes);
			g_LoadedOverrideFiles.push_back(SimGroupTOC);

			RecoverPtr<RWS::CAttributePacket*>(SimGroupTOC->m_EntPackets, (uint8_t*)SimGroupTOC);
			for (uint32_t idx = 0; idx < SimGroupTOC->m_NumEnts; idx++)
			{
				RecoverPtr<RWS::CAttributePacket>(SimGroupTOC->m_EntPackets[idx], (uint8_t*)SimGroupTOC);
				RWS::CAttributePacket* Pckt = SimGroupTOC->m_EntPackets[idx];

				const EARS::Common::guid128_t PcktID = Pckt->GetInstanceID();
				EARS::Diag::Printf("Loaded behaviour [0x%X-0x%X-0x%X-0x%X] from SimGroupOverride file [%s]", PcktID[0], PcktID[1], PcktID[2], PcktID[3], AsPath.c_str());

				g_RegisteredPackets.insert({ PcktID, Pckt });
			}

			EARS::Diag::Printf("Finished SimGroupOverride file [%s], with a total of %u behaviours mounted.", AsPath.c_str(), SimGroupTOC->m_NumEnts);
		}
	}

	atexit(DestroyTOC);

	// Installed last: SimManager starts consulting it the moment it is set, and
	// the table has to be complete by then.
	EARS::ModPoints::SetResolveAttributePacket(&ResolveOverridePacket);
}
