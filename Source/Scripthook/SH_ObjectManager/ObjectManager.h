#pragma once

// RenderWare Framework
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

// Forward declares
namespace RWS { class CAttributePacket; }

namespace Mod
{
	class ObjectManager : public RWS::CEventHandler
	{
	public:

		// NB: EXPERIMENTAL CODE
		// Spawn an object in the game world
		void Spawn(const RWS::CAttributePacket& AttrPacket);

		// NB: EXPERIMENTAL CODE
		// Spawn an object in the game world
		void Spawn(const EARS::Common::guid128_t& PacketID);

	private:
	};
}
