#include "ObjectManager.h"

// Addons
#include <Addons/Hook.h>

// SDK
#include "SDK/EARS_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Core/SimManager/SimManager.h"
#include "SDK/EARS_Framework/Core/Entity/Entity.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

void Mod::ObjectManager::Spawn(const RWS::CAttributePacket& AttrPacket, const RwV3d Position)
{
	uint32_t ClassID = AttrPacket.GetIdOfClassToCreate();
	RWS::CAttributeHandler* NewHandler = MemUtils::CallCdeclMethod<RWS::CAttributeHandler*, uint32_t, const RWS::CAttributePacket*>(0x0483410, ClassID, &AttrPacket);

	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	SimMgr->SendPostSpawnInitializeToEntity(NewHandler, true);

	if (EARS::Framework::Entity* EntityHandler = reinterpret_cast<EARS::Framework::Entity*>(NewHandler))
	{
		EntityHandler->SetPosition(Position);
	}
}

void Mod::ObjectManager::Spawn(const EARS::Common::guid128_t& PacketID, const RwV3d Position)
{
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	if (RWS::CAttributePacket* FoundPacket = SimMgr->GetAttributePacket(&PacketID, 0))
	{
		Spawn(*FoundPacket, Position);
	}
}
