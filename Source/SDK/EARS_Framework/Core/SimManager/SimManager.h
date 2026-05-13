#pragma once

// RenderWare Framework
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/HashTable.h"
#include "SDK/EARS_Common/DoubleInternalLinkedList.h"
#include "SDK/EARS_Common/ManagedArray.h"
#include "SDK/EARS_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/ResourceManager/CResourceHandler.h"

// C++
#include <functional>

// forward declares
namespace RWS
{
	class CAttributePacket;
}

namespace EARS
{
	namespace Framework
	{
		/**
		 * Defines a SimGroup structure the game uses to generate entities from packets 
		 */
		struct SimGroupTOC : public EARS::Common::DoubleLinkedListNodeMixin<SimGroupTOC>
		{
		public:

			uint32_t m_MagicNumber = 0;
			uint32_t m_VersionOrDispatchTime = 0;
			EARS::Common::guid32_t m_Guid; // actually zero
			uint32_t m_Flags = 0;
			uint32_t m_NumEnts = 0;
			uint32_t m_NumEntsDispatched = 0;
			uint32_t m_SharedDataSize = 0;
			uint32_t m_DictionarySize = 0;
			RWS::CAttributePacket** m_EntPackets = nullptr;
			uint32_t m_hStream = 0;
		};

		// TODO: Implement Singleton base class
		class SimManager : public RWS::CEventHandler, public RWS::CResourceHandler/*, public Singleton<CResourceManager>*/
		{
		public:

			// Fetch an Attribute Packet using the specified GUID.
			// This searches through the loaded SimGroupTOCs.
			RWS::CAttributePacket* GetAttributePacket(const EARS::Common::guid128_t* InGuid, int bMaskStream);

			// Fetch an active handler for the specified packet
			RWS::CAttributeHandler* Find(const EARS::Common::guid128_t& InstanceID, RWS::CAttributeHandler* StartHandler);
			
			// Check whether a SimGroup is currently being overriden.
			// Result is the index into the override array.
			int FindSimGroupOverride(const EARS::Common::guid32_t& SimGroupGuid) const;

			// Check whether a SimGroup has permission to dispatch.
			bool SimGroupDispatchEnabled(const SimGroupTOC& SimGroupTOC) const;

			// NB: EXPERIMENTAL CODE
			// Spawn an object in the game world
			void* SpawnEntity(const EARS::Common::guid128_t* InGuid, int SpawnFlags);

			// NB: EXPERIMENTAL CODE
			// Spawn an object in the game world
			void* SpawnEntity(RWS::CAttributePacket* Packet, uint32_t StreamHandle, bool bSkipPostInit);

			// NB: EXPERIMENTAL CODE
			// Triggers PostSpawnInitialize message for the Handler passed into the function
			void SendPostSpawnInitializeToEntity(RWS::CAttributeHandler* Handler, bool bSendToInactive);

			// NB: ROUTED FROM HOOK, NOT CURRENTLY CALLED. SHOULD BE PART OF VTABLE
			void LoadResource(RWS::CResourceHandler::CResourceLoadInfo* LoadInfo);

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

			enum class SimGroupOverrideFlags : int32_t
			{
				OVERRIDE_FORCE_ENABLE = 0x1,
				OVERRIDE_FORCE_DISABLE = 0x2,
				IGNORE_DISPATCH_LOCK = 0x4,
			};

			struct SimGroupOverride
			{
				EARS::Common::guid32_t m_SimGroupGUID;
				uint32_t m_OverrideFlags = 0;
			};

			char m_SimManagerPadding_0[0x24];
			uint32_t m_MaxSpawnLatencyMSec = 0;
			char m_SimManagerPadding_1[0x2C];
			RWS::CAttributePacketEntityList m_OrphanedEntityList;
			RWS::CAttributePacketEntityList m_HiddenEntityList;

			// TODO: This is fairly messy
			DEFINE_MEMBER_IntrusiveHashTable(EARS::Common::guid128_t, RWS::CAttributePacket, AttrPacketGetKey, EARS::Common::HashNext<RWS::CAttributePacket>, m_AttributePacketHash);
			EARS::Common::IntrusiveHashTable<EARS::Common::guid128_t, RWS::CAttributeHandler> m_AttributeHandlerHash;
			EARS::Common::DoubleInternalLinkedList<EARS::Framework::SimGroupTOC> m_SimGroupListArr[4];
			ManagedArray<SimGroupOverride> m_SimGroupOverrides;
		};

		static_assert(sizeof(SimManager) == 196); // actually much bigger
	}
}

