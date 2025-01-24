#include "ObjectManager.h"

// Addons
#include <Addons/Hook.h>

// SDK (Framework)
#include "SDK/EARS_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Core/Entity/Entity.h"
#include "SDK/EARS_Framework/Core/SimManager/SimManager.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"

// SDK (Godfather)
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

namespace DefinedEvents
{
	static hook::Type<RWS::CEventId> iMsgStreamLoadCompleted = hook::Type<RWS::CEventId>(0x1206760);
}

void Mod::ObjectManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	RWS::CEventHandler::HandleEvents(MsgEvent);

	if (MsgEvent.IsEvent(DefinedEvents::iMsgStreamLoadCompleted))
	{
		const EARS::Framework::StreamEvent* EventData = MsgEvent.GetDataAs<EARS::Framework::StreamEvent>();

		Spawn(CurrentRequestParams.ObjectGuid, CurrentRequestParams.DesiredPosition);
		CurrentRequestParams = {};

		UnlinkMsg(&DefinedEvents::iMsgStreamLoadCompleted);
	}
}

void Mod::ObjectManager::Spawn(const RWS::CAttributePacket& AttrPacket, const RwV3d& Position)
{
	uint32_t ClassID = AttrPacket.GetIdOfClassToCreate();
	RWS::CAttributeHandler* NewHandler = MemUtils::CallCdeclMethod<RWS::CAttributeHandler*, uint32_t, const RWS::CAttributePacket*>(0x0483410, ClassID, &AttrPacket);

	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	SimMgr->SendPostSpawnInitializeToEntity(NewHandler, true);

	// TODO: Do not assume that this is an entity, some objects do not inherit entity
	if (EARS::Framework::Entity* EntityHandler = reinterpret_cast<EARS::Framework::Entity*>(NewHandler))
	{
		EntityHandler->SetPosition(Position);
	}
}

void Mod::ObjectManager::SpawnItem(const EARS::Common::guid128_t& PacketID, const RwV3d& Position)
{
	RWS::CAttributeHandler* Result = MemUtils::CallCdeclMethod<RWS::CAttributeHandler*, const EARS::Common::guid128_t&, unsigned int, bool, const EARS::Common::guid128_t*, bool>(0x0856780, PacketID, 0, false, nullptr, true);

	// TODO: Do not assume that this is an entity, some objects do not inherit entity
	if (EARS::Framework::Entity* EntityHandler = reinterpret_cast<EARS::Framework::Entity*>(Result))
	{
		EntityHandler->SetPosition(Position);
	}
}

bool Mod::ObjectManager::RequestSpawnCar(const ObjectSpawnRequestParams& RequestParams)
{
	// fetch packet from sim manager
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	const RWS::CAttributePacket* Packet = SimMgr->GetAttributePacket(&RequestParams.ObjectGuid, 0);
	if (Packet == nullptr)
	{
		return false;
	}

	// check whether its a car
	if (Packet->GetIdOfClassToCreate() != 0x10E5319E)
	{
		return false;
	}

	// now we need to check whether the dependency stream is loaded
	// this is stored in Animated packet command buffer
	RWS::CAttributeCommandIterator AnimatedIt = RWS::CAttributeCommandIterator::MakeIterator(*Packet, 0xAE986323);
	AnimatedIt.SeekTo(5);

	// NB: This is Stream GUID!
	// TODO: Should check whether the guid is actually valid?
	const EARS::Common::guid128_t* RWSGuid = AnimatedIt->GetAs_RWS_GUID();
	const uint32_t AsGuid32 = RWSGuid->GetGuid32();

	EARS::Framework::StreamManager* StreamMgr = EARS::Framework::StreamManager::GetInstance();
	const uint32_t StreamHandle = StreamMgr->GetStreamHandle(AsGuid32);
	if (StreamHandle == 0 || StreamMgr->GetStatus(StreamHandle) != EARS::Framework::STREAM_STATUS_DISPATCHED)
	{
		// probably not loaded
		const uint32_t StreamHandle = StreamMgr->Load(AsGuid32, 0.0f, 0);

		// link to recieve event
		StreamMgr->LinkStreamMsg(2, *this, 0x8000);

		// store as we have deferred
		CurrentRequestParams = RequestParams;
	}
	else
	{
		// we can spawn!
		Spawn(RequestParams.ObjectGuid, RequestParams.DesiredPosition);
	}

	return true;
}

void Mod::ObjectManager::ForEachItem(const TVisitEntityFunctor& InFunction)
{
	ItemSpawnList.ForEachEntry([&](const EntityEntry& Entry)
		{
			InFunction(Entry.Name, Entry.GUID);
		});
}

void Mod::ObjectManager::Spawn(const EARS::Common::guid128_t& PacketID, const RwV3d& Position)
{
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	if (RWS::CAttributePacket* FoundPacket = SimMgr->GetAttributePacket(&PacketID, 0))
	{
		Spawn(*FoundPacket, Position);
	}
}

void Mod::ObjectManager::ImGuiDrawContents()
{
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer();

	if (ImGui::Button("Populate lists"))
	{
		VehicleSpawnList.ClearList();
		NPCSpawnList.ClearList();
		ItemSpawnList.ClearList();

		SimMgr->ForEachPacket([&](RWS::CAttributePacket& Packet)
			{
				const uint32_t ClassID = Packet.GetIdOfClassToCreate();
				if (ClassID == 0xD7E44D6A)
				{
					EntityEntry NewEntry = {};
					NewEntry.GUID = Packet.GetInstanceID();

					// We need to get NPC name from active SimNPC
					const auto& EntityIt = Packet.GetEntityIterator();
					if (EntityIt.IsFinished() == false)
					{
						const RWS::CAttributeHandler* CurrentEntity = EntityIt.GetEntity();
						assert(CurrentEntity->HasAttributeHandlerFlag(0x8000000));

						const EARS::Framework::Base* AsBase = reinterpret_cast<const EARS::Framework::Base*>(CurrentEntity);
						EARS::Modules::SimNPC* AsSimNPC = EARS::Framework::_QueryInterface<EARS::Modules::SimNPC>(AsBase, 0xD7E44D6A);
						String* NPCName = AsSimNPC->GetName();

						NewEntry.Name = NPCName->c_str();
					}
					else
					{
						// TODO: If the SimNPC does not exist, perhaps we should fetch the NPC name from the NPC packet?
						int z= 0;
					}

					NPCSpawnList.RegisterEntry(NewEntry);
				}
				else if (ClassID == 0x10E5319E)
				{
					RWS::CAttributeCommandIterator PacketIt = RWS::CAttributeCommandIterator::MakeIterator(Packet, 0xA5975EB2);
					PacketIt.SeekTo(0x55); // seek straight to parts name

					// create entity and fetch name
					EntityEntry NewEntry = {};
					NewEntry.GUID = Packet.GetInstanceID();
					NewEntry.Name = PacketIt->GetAs_char_ptr();

					VehicleSpawnList.RegisterEntry(NewEntry);
				}
				else if (ClassID == 0xF26FB813 || ClassID == 0x332D5A20 || ClassID == 0x4ECFBE13)
				{
					RWS::CAttributeCommandIterator PacketIt = RWS::CAttributeCommandIterator::MakeIterator(Packet, 0x4ECFBE13);
					PacketIt.SeekTo(0); // seek straight to item name

					EntityEntry NewEntry = {};
					NewEntry.GUID = Packet.GetInstanceID();
					NewEntry.Name = PacketIt->GetAs_char_ptr();

					ItemSpawnList.RegisterEntry(NewEntry);
				}
			});
	}

	if (ImGui::CollapsingHeader("Vehicles", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("vehicle_spawn_list");

		VehicleSpawnList.DrawList();

		if (ImGui::Button("Spawn Car"))
		{
			Mod::ObjectManager::ObjectSpawnRequestParams RequestParams = {};
			RequestParams.ObjectGuid = VehicleSpawnList.GetSelectedGUID();

			// build position so its in front of player
			const RwMatrixTag PlayerMatrix = LocalPlayer->GetMatrix();
			RequestParams.DesiredPosition = PlayerMatrix.m_Pos + (PlayerMatrix.m_At * 5.0f);

			RequestSpawnCar(RequestParams);
		}

		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("NPCs", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("npc_spawn_list");
		NPCSpawnList.DrawList();

		if (ImGui::Button("Spawn NPC"))
		{
			const RWS::CAttributeHandler* ActiveSimNPC = SimMgr->Find(NPCSpawnList.GetSelectedGUID(), nullptr);
			assert(ActiveSimNPC->HasAttributeHandlerFlag(0x8000000));

			if (ActiveSimNPC)
			{
				const EARS::Framework::Base* AsBase = reinterpret_cast<const EARS::Framework::Base*>(ActiveSimNPC);
				const EARS::Modules::SimNPC* AsSimNPC = EARS::Framework::_QueryInterface<EARS::Modules::SimNPC>(AsBase, 0xD7E44D6A);
				const EARS::Common::guid128_t NPCGuid = AsSimNPC->GetNPCGuid();

				const RwMatrixTag PlayerMatrix = LocalPlayer->GetMatrix();
				const RwV3d SpawnPosition = PlayerMatrix.m_Pos + (PlayerMatrix.m_At * 5.0f);
				Spawn(NPCGuid, SpawnPosition);
			}
		}

		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Items", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("item_spawn_list");
		ItemSpawnList.DrawList();

		ImGui::PushItemWidth(-1.0f);
		if (ImGui::Button("Spawn Item"))
		{
			const RwMatrixTag PlayerMatrix = LocalPlayer->GetMatrix();
			const RwV3d SpawnPosition = PlayerMatrix.m_Pos + (PlayerMatrix.m_At * 5.0f);
			SpawnItem(ItemSpawnList.GetSelectedGUID(), SpawnPosition);
		}
		ImGui::PopItemWidth();

		ImGui::PopID();
	}
}

Mod::ObjectManager::ObjectEntryList::ObjectEntryList()
{
	Entries.reserve(1024);
	FilteredEntries.reserve(512);
}

Mod::ObjectManager::ObjectEntryList::~ObjectEntryList()
{
	ClearList();
}

void Mod::ObjectManager::ObjectEntryList::RegisterEntry(const EntityEntry& InEntry)
{
	Entries.push_back(InEntry);
}

void Mod::ObjectManager::ObjectEntryList::DrawList()
{
	ObjectFilter.Draw("##filter");
	ImGui::SameLine();
	if (ImGui::Button("Search"))
	{
		RequestSearch();
	}

	ImGui::PushItemWidth(-1.0f);
	if (ImGui::BeginListBox("##list", ImVec2(0.0f, 0.0f)))
	{
		const size_t SelectedVectorSize = (bSearchModeActive == true ? FilteredEntries.size() : Entries.size());

		ImGuiListClipper Clipper;
		Clipper.Begin(SelectedVectorSize);
		while (Clipper.Step())
		{
			for (int i = Clipper.DisplayStart; i < Clipper.DisplayEnd; i++)
			{
				ImGui::PushID(i);

				const EntityEntry& CurrentEntry = (bSearchModeActive ? Entries[FilteredEntries[i]] : Entries[i]);
				if (ImGui::Selectable(CurrentEntry.Name.data(), (SelectedGuid == CurrentEntry.GUID)))
				{
					SelectedGuid = CurrentEntry.GUID;
				}

				ImGui::PopID();
			}
		}

		Clipper.End();

		ImGui::EndListBox();
	}
	ImGui::PopItemWidth();
}

void Mod::ObjectManager::ObjectEntryList::ClearList()
{
	Entries.clear();
	FilteredEntries.clear();
	SelectedGuid = {};
}

void Mod::ObjectManager::ObjectEntryList::ForEachEntry(const TVisitEntityFunctor& InFunction)
{
	for (const EntityEntry& Entry : Entries)
	{
		InFunction(Entry);
	}
}

void Mod::ObjectManager::ObjectEntryList::RequestSearch()
{
	FilteredEntries.clear();

	// only attempt to search if
	if (ObjectFilter.IsActive())
	{
		for (size_t i = 0; i < Entries.size(); i++)
		{
			if (ObjectFilter.PassFilter(Entries[i].Name.data()))
			{
				FilteredEntries.push_back(i);
			}
		}
	}

	bSearchModeActive = (FilteredEntries.size() > 0);
}
