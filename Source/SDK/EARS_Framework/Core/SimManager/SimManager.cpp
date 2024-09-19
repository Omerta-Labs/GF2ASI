#include "SimManager.h"

// Addons
#include <Addons/Hook.h>

RWS::CAttributePacket* EARS::Framework::SimManager::GetAttributePacket(const EARS::Common::guid128_t* InGuid, int bMaskStream)
{
	return MemUtils::CallClassMethod<RWS::CAttributePacket*, SimManager*, const EARS::Common::guid128_t*, int>(0x04461C0, this, InGuid, bMaskStream);
}

void* EARS::Framework::SimManager::SpawnEntity(const EARS::Common::guid128_t* InGuid, int SpawnFlags)
{
	return MemUtils::CallClassMethod<void*, SimManager*, const EARS::Common::guid128_t*, int>(0x0446130, this, InGuid, SpawnFlags);
}

void EARS::Framework::SimManager::SendPostSpawnInitializeToEntity(RWS::CAttributeHandler* Handler, bool bSendToInactive)
{
	MemUtils::CallClassMethod<void, SimManager*, RWS::CAttributeHandler*, bool>(0x0446470, this, Handler, bSendToInactive);
}

EARS::Framework::SimManager* EARS::Framework::SimManager::GetInstance()
{
	// 
	return *(SimManager**)0x1223410;
}

void* EARS::Framework::SimManager::SpawnEntity(RWS::CAttributePacket* Packet, uint32_t StreamHandle, bool bSkipPostInit)
{
	return MemUtils::CallClassMethod<void*, SimManager*, RWS::CAttributePacket*, uint32_t, bool>(0x0446340, this, Packet, StreamHandle, bSkipPostInit);
}
