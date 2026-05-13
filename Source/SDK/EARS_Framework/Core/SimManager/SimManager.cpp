#include "SimManager.h"

// Addons
#include <Addons/tConsole.h>
#include <Addons/Hook.h>

// SDK
#include "SDK/EARS_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"

namespace PrivateUtils
{
	template<typename TClass>
	void RecoverPtr(TClass*& PtrToFixUp, const uint8_t* PtrBase)
	{
		if (PtrToFixUp != nullptr)
		{
			uintptr_t offset = reinterpret_cast<uintptr_t>(PtrToFixUp);
			uintptr_t base = reinterpret_cast<uintptr_t>(PtrBase);
			PtrToFixUp = reinterpret_cast<TClass*>(offset + base);
		}
	}
}

RWS::CAttributePacket* EARS::Framework::SimManager::GetAttributePacket(const EARS::Common::guid128_t* InGuid, int bMaskStream)
{
	return MemUtils::CallClassMethod<RWS::CAttributePacket*, SimManager*, const EARS::Common::guid128_t*, int>(0x04461C0, this, InGuid, bMaskStream);
}

RWS::CAttributeHandler* EARS::Framework::SimManager::Find(const EARS::Common::guid128_t& InstanceID, RWS::CAttributeHandler* StartHandler)
{
	return MemUtils::CallClassMethod<RWS::CAttributeHandler*, EARS::Framework::SimManager*, const EARS::Common::guid128_t&, RWS::CAttributeHandler*>(0x0445FF0, this, InstanceID, StartHandler);
}

int EARS::Framework::SimManager::FindSimGroupOverride(const EARS::Common::guid32_t& Guid) const
{
	for (int32_t idx = 0; idx < m_SimGroupOverrides.Size(); idx++)
	{
		const SimGroupOverride* CurrentOverride = m_SimGroupOverrides[idx];
		if (CurrentOverride->m_SimGroupGUID == Guid)
		{
			return idx;
		}
	}

	return -1;
}

bool EARS::Framework::SimManager::SimGroupDispatchEnabled(const SimGroupTOC& SimGroupTOC) const
{
	const int32_t OverrideIdx = FindSimGroupOverride(SimGroupTOC.m_Guid);
	if (OverrideIdx == -1)
	{
		// SIM_GROUP_DEFAULT_ACTIVE
		return ((SimGroupTOC.m_Flags & 1) != 0);
	}

	const SimGroupOverride* OverrideInst = m_SimGroupOverrides[OverrideIdx];
	if ((OverrideInst->m_OverrideFlags & (int)SimGroupOverrideFlags::OVERRIDE_FORCE_ENABLE) != 0)
	{
		return true;
	}
	else if ((OverrideInst->m_OverrideFlags & (int)SimGroupOverrideFlags::OVERRIDE_FORCE_DISABLE) != 0)
	{
		return false;
	}

	return false;
}

void* EARS::Framework::SimManager::SpawnEntity(const EARS::Common::guid128_t* InGuid, int SpawnFlags)
{
	return MemUtils::CallClassMethod<void*, SimManager*, const EARS::Common::guid128_t*, int>(0x0446130, this, InGuid, SpawnFlags);
}

void EARS::Framework::SimManager::SendPostSpawnInitializeToEntity(RWS::CAttributeHandler* Handler, bool bSendToInactive)
{
	MemUtils::CallClassMethod<void, SimManager*, RWS::CAttributeHandler*, bool>(0x0446470, this, Handler, bSendToInactive);
}

void EARS::Framework::SimManager::LoadResource(RWS::CResourceHandler::CResourceLoadInfo* LoadInfo)
{
	EARS::Framework::StreamManager* StrmMgr = EARS::Framework::StreamManager::GetInstance();

	const uint32_t DispatchStream = StrmMgr->GetDispatchStream();
	assert(DispatchStream != 0);

	const char* StreamName = StrmMgr->GetFilename(DispatchStream);
	assert(StreamName && *StreamName);

	EARS::Framework::SimGroupTOC* SimGroupTOC = (EARS::Framework::SimGroupTOC*)LoadInfo->BindResourceData();
	assert(SimGroupTOC);

	if (SimGroupTOC->m_MagicNumber != 0x53696D47 || SimGroupTOC->m_VersionOrDispatchTime != 1)
	{
		// failure
		return;
	}

	hook::Type<uint32_t> some_value = hook::Type<uint32_t>(0x1163EC0);

	LARGE_INTEGER PerformanceCount;
	QueryPerformanceCounter(&PerformanceCount);
	SimGroupTOC->m_VersionOrDispatchTime = (1000 * (PerformanceCount.QuadPart - m_MaxSpawnLatencyMSec) / some_value);

	tConsole::fPrintf("SimManager::LoadResource: [%s]", StreamName);

	PrivateUtils::RecoverPtr<RWS::CAttributePacket*>(SimGroupTOC->m_EntPackets, (uint8_t*)SimGroupTOC);

	for (uint32_t idx = 0; idx < SimGroupTOC->m_NumEnts; idx++)
	{
		PrivateUtils::RecoverPtr<RWS::CAttributePacket>(SimGroupTOC->m_EntPackets[idx], (uint8_t*)SimGroupTOC);

		RWS::CAttributePacket* Pckt = SimGroupTOC->m_EntPackets[idx];

		int z = 0;
	}

	SimGroupTOC->m_hStream = DispatchStream;

	bool bNeedsDispatchLock = SimGroupDispatchEnabled(*SimGroupTOC);
	if (bNeedsDispatchLock)
	{
		const int32_t OverrideIdx = FindSimGroupOverride(SimGroupTOC->m_Guid);
		if (OverrideIdx >= 0 && ((m_SimGroupOverrides[OverrideIdx]->m_OverrideFlags & (int)SimGroupOverrideFlags::IGNORE_DISPATCH_LOCK) == 0))
		{
			bNeedsDispatchLock = false;
		}

		if (bNeedsDispatchLock)
		{
			(&m_SimGroupListArr[0])->InsertAtBack(SimGroupTOC);

			SimGroupTOC->m_Flags |= 0x20;
			StrmMgr->AddDispatchLockComplete();
		}
		else
		{
			SimGroupTOC->m_Flags |= 0x100;
			(&m_SimGroupListArr[0])->InsertAtBack(SimGroupTOC);
		}
	}
	else
	{
		(&m_SimGroupListArr[0])->InsertAtBack(SimGroupTOC);
	}
}

void EARS::Framework::SimManager::ForEachPacket(const EARS::Framework::SimManager::TPacketVisitor& VisitorFunc)
{
	auto PacketIt = m_AttributePacketHash.CreateIterator();
	while (PacketIt.IsFinshed() == false)
	{
		RWS::CAttributePacket* CurPacket = *PacketIt;
		VisitorFunc(*CurPacket);

		PacketIt++;
	}
}

EARS::Framework::SimManager* EARS::Framework::SimManager::GetInstance()
{
	return *(SimManager**)0x1223410;
}

void* EARS::Framework::SimManager::SpawnEntity(RWS::CAttributePacket* Packet, uint32_t StreamHandle, bool bSkipPostInit)
{
	return MemUtils::CallClassMethod<void*, SimManager*, RWS::CAttributePacket*, uint32_t, bool>(0x0446340, this, Packet, StreamHandle, bSkipPostInit);
}

const EARS::Common::guid128_t& EARS::Framework::SimManager::AttrPacketGetKey::GetKey(const RWS::CAttributePacket* InPacket)
{
	return InPacket->GetInstanceID();
}
