#pragma once

// ObjectManager base class
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

// Addons
#include <Addons/imgui/imgui.h>

// RenderWare Framework
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/RwMaths.h"

// CPP
#include <functional>
#include <string>
#include <vector>

// Forward declares
namespace RWS { class CAttributePacket; }

namespace Mod
{
	class ObjectManager : public RWS::CEventHandler
	{
	public:

		struct ObjectSpawnRequestParams
		{
		public:

			EARS::Common::guid128_t ObjectGuid;

			RwV3d DesiredPosition;

		};

		//~ Begin RWS::CEventHandler Interface
		virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
		//~ End RWS::CEventHandler Interface

		// Render the contents of the Object Manager into ImGui
		void ImGuiDrawContents();

		// NB: EXPERIMENTAL CODE
		// Spawn an object in the game world
		void Spawn(const RWS::CAttributePacket& AttrPacket, const RwV3d& Position);

		// NB: EXPERIMENTAL CODE
		// Spawn an object in the game world
		void Spawn(const EARS::Common::guid128_t& PacketID, const RwV3d& Position);

		// NB: EXPERIMENTAL CODE
		// Spawn an object in the game world
		void SpawnItem(const EARS::Common::guid128_t& PacketID, const RwV3d& Position);

		// NB: EXPERIMENTAL CODE
		// Spawn an object in the game world
		bool RequestSpawnCar(const ObjectSpawnRequestParams& RequestParams);

		// Iterate through all items and run the supplied function
		typedef std::function<void(const std::string&, const EARS::Common::guid128_t&)> TVisitEntityFunctor;
		void ForEachItem(const TVisitEntityFunctor& InFunction);

	private:

		// current spawn parameters for a deferred item.
		ObjectSpawnRequestParams CurrentRequestParams;

		struct EntityEntry
		{
			// TODO: Can we use games String*?
			// Reduce duplicate text then.
			std::string Name;

			// the Attribute packet GUID for the entity.
			EARS::Common::guid128_t GUID;
		};

		/**
		 * Internal object list for specific type
		 * Only usable by object manager
		 */
		struct ObjectEntryList final
		{
		public:

			ObjectEntryList();
			~ObjectEntryList();

			// register an entity with the entity list
			// does not check for duplicates!
			void RegisterEntry(const EntityEntry& InEntry);

			// render list in ImGui, includes search, filters and list.
			void DrawList();

			// clears and empties the object entry list
			void ClearList();

			// Iterate through all entries within the list
			typedef std::function<void(const EntityEntry& Entry)> TVisitEntityFunctor;
			void ForEachEntry(const TVisitEntityFunctor& InFunction);

			// get the selected guid from the list
			EARS::Common::guid128_t GetSelectedGUID() const { return SelectedGuid; }

		private:

			// trigger a search using the current filters
			void RequestSearch();

			std::vector<EntityEntry> Entries;
			std::vector<uint32_t> FilteredEntries;
			EARS::Common::guid128_t SelectedGuid;
			ImGuiTextFilter ObjectFilter;
			bool bSearchModeActive = false;
		};

		// Entity spawn lists
		ObjectEntryList VehicleSpawnList;
		ObjectEntryList NPCSpawnList;
		ObjectEntryList ItemSpawnList;
	};
}
