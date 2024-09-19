#pragma once

// RenderWare Framework
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Forward declares
namespace RWS { class CAttributePacket; class CAttributeHandler; }

namespace EARS
{
	namespace Framework
	{
		class SimManager
		{
		public:

			// Fetch an Attribute Packet using the specified GUID.
			// This searches through the loaded SimGroupTOCs.
			RWS::CAttributePacket* GetAttributePacket(const EARS::Common::guid128_t* InGuid, int bMaskStream);

			// NB: EXPERIMENTAL CODE
			// Spawn an object in the game world
			void* SpawnEntity(const EARS::Common::guid128_t* InGuid, int SpawnFlags);

			// NB: EXPERIMENTAL CODE
			// Spawn an object in the game world
			void* SpawnEntity(RWS::CAttributePacket* Packet, uint32_t StreamHandle, bool bSkipPostInit);

			// NB: EXPERIMENTAL CODE
			// Triggers PostSpawnInitialize message for the Handler passed into the function
			void SendPostSpawnInitializeToEntity(RWS::CAttributeHandler* Handler, bool bSendToInactive);

			// Fetch the SimManager instance
			static SimManager* GetInstance();
		};
	}
}

