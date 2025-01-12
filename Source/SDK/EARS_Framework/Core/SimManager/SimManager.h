#pragma once

// RenderWare Framework
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/HashTable.h"
#include "SDK/EARS_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/ResourceManager/CResourceHandler.h"

// C++
#include <functional>

namespace EARS
{
	namespace Framework
	{
		// TODO: Implement Singleton base class
		class SimManager : public RWS::CEventHandler, public RWS::CResourceHandler/*, public Singleton<CResourceManager>*/
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

			typedef std::function<void(RWS::CAttributePacket&)> TPacketVisitor;
			void ForEachPacket(const TPacketVisitor& VisitorFunc);

			// Fetch the SimManager instance
			static SimManager* GetInstance();

		private:

			struct AttrPacketGetKey
			{
				// used to request instance ID from packet inside a IntrusiveHashTable
				static const EARS::Common::guid128_t& GetKey(const RWS::CAttributePacket* InPacket);
			};

			char m_SimManagerPadding_0[0x54];

			RWS::CAttributePacketEntityList m_OrphanedEntityList;
			RWS::CAttributePacketEntityList m_HiddenEntityList;

			// TODO: This is fairly messy
			DEFINE_MEMBER_IntrusiveHashTable(EARS::Common::guid128_t, RWS::CAttributePacket, AttrPacketGetKey, EARS::Common::HashNext<RWS::CAttributePacket>, m_AttributePacketHash);
			EARS::Common::IntrusiveHashTable<EARS::Common::guid128_t, RWS::CAttributeHandler> m_AttributeHandlerHash;
		};
	}
}

