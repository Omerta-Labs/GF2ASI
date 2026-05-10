#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"
#include "Addons/Settings.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"
#include "Scripthook/SH_ObjectManager/ObjectManager.h"

// Godfather
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Core/SimManager/SimManager.h"
#include "SDK/EARS_Framework/Core/StreamManager/StreamManager.h"
#include "SDK/EARS_Framework/Toolkits/GroupManager/GroupManager.h"
#include "SDK/EARS_Godfather/Modules/Families/Family.h"
#include "SDK/EARS_Godfather/Modules/Families/FamilyManager.h"
#include "SDK/EARS_Godfather/Modules/Families/CorleoneData.h"
#include "SDK/EARS_Godfather/Modules/Families/MadeMan.h"
#include "SDK/EARS_Godfather/Modules/Item/Inventory.h"
#include "SDK/EARS_Godfather/Modules/Item/InventoryManager.h"
#include "SDK/EARS_Godfather/Modules/Item/Item.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDebug.h"
#include "SDK/EARS_Godfather/Modules/Mobface/MobfaceManager.h"
#include "SDK/EARS_Godfather/Modules/TimeOfDay/TimeOfDayManager.h"
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/NPC/Crime/CrimeManager.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegion.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegionManager.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/Missions/CheckpointManager.h"
#include "SDK/EARS_Godfather/Modules/Missions/Checkpoint.h"
#include "SDK/EARS_Godfather/Modules/UI/UIHud.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/Behaviours/WhiteboxCar/WhiteboxCar.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/VehicleDamageComponent.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"
#include "SDK/EARS_Physics/Vehicles/ground/wheeled/HavokWheeledVehicle.h"
#include "SDK/EARS_Locale/LocaleManager.h"

#include "SDK/EARS_RT_LLRender/include/ShaderManager.h"

// CPP
#include <iostream>
#include <fstream>
#include <format>
#include <string>
#include <sstream>

#define ENABLE_ENTITY_SPAWN_DEBUG 0

#define SHOW_ATTRIBUTEPACKET_WINDOW 0

#if DEBUG
#define SHOW_DEMOGRAPHICS_TAB 0
#define SHOW_FAMILY_TAB 1
#else
#define SHOW_DEMOGRAPHICS_TAB 0
#define SHOW_FAMILY_TAB 1
#endif // DEBUG

#if ENABLE_ENTITY_SPAWN_DEBUG
class NPCManager
{
public:

	void* Create(const EARS::Common::guid128_t& InGuid, uint32_t InPriority, void* InOwner, uint32_t InHStream)
	{
		return MemUtils::CallClassMethod<void*, NPCManager*, const EARS::Common::guid128_t&, uint32_t, void*, uint32_t>(
			0x08F0BB0, this, InGuid, InPriority, InOwner, InHStream);
	}

	static NPCManager* GetInstance()
	{
		// 
		return *(NPCManager**)0x112FDD4;
	}
};
#endif // ENABLE_ENTITY_SPAWN_DEBUG

namespace DefinedEvents
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	static hook::Type<RWS::CEventId> DoRenderEvent = hook::Type<RWS::CEventId>(0x01206970);
	static hook::Type<RWS::CEventId> PreRenderEvent = hook::Type<RWS::CEventId>(0x01206980);
	static hook::Type<RWS::CEventId> PlayerAsDriverEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E030);
	static hook::Type<RWS::CEventId> PlayerAsPassengerEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E11C);
	static hook::Type<RWS::CEventId> PlayerExitVehicleEvent = hook::Type<RWS::CEventId>(0x112E018);

	static hook::Type<RWS::CEventId> iMsgStreamLoadComplete = hook::Type<RWS::CEventId>(0x1206760);
	static hook::Type<RWS::CEventId> iMsgStreamUnloadComplete = hook::Type<RWS::CEventId>(0x1206768);
	static hook::Type<RWS::CEventId> iMsgStreamBeginUnload = hook::Type<RWS::CEventId>(0x1206778);
	static hook::Type<RWS::CEventId> iMsgStreamCancel = hook::Type<RWS::CEventId>(0x1206780);
	static hook::Type<RWS::CEventId> iMsgStreamAllDispatched = hook::Type<RWS::CEventId>(0x120678C);
	static hook::Type<RWS::CEventId> iMsgStreamBeginLoad = hook::Type<RWS::CEventId>(0x1206794);
	static hook::Type<RWS::CEventId> iMsgStreamIdle = hook::Type<RWS::CEventId>(0x12067A4);
	static hook::Type<RWS::CEventId> iMsgStreamUnloading = hook::Type<RWS::CEventId>(0x12067B4);
}

ImGuiManager::ImGuiManager()
	: CEventHandler()
{

}

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX9_Shutdown();
}

void ImGuiManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	RWS::CEventHandler::HandleEvents(MsgEvent);

	if (MsgEvent.IsEvent(DefinedEvents::RunningTickEvent))
	{
		OnTick();
	}

	if (MsgEvent.IsEvent(DefinedEvents::PlayerAsDriverEnterVehicleEvent))
	{
		// On Vehicle entered
	}

	if (MsgEvent.IsEvent(DefinedEvents::PlayerAsPassengerEnterVehicleEvent))
	{
		// On Vehicle entered
	}

	if (MsgEvent.IsEvent(DefinedEvents::PlayerExitVehicleEvent))
	{
		if (bPlayerVehicleGodModeActive)
		{
			// try and disable god mode
			if (const EARS::Modules::Player* const LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
			{
				SetVehicleGodMode(LocalPlayer->GetVehicle(), false);
				bPlayerVehicleGodModeActive = false;
			}
		}
	}
}

void ImGuiManager::Open()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	hook::Type<HWND> windowHandle = hook::Type<HWND>(0x112A024);
	ImGui_ImplWin32_Init(windowHandle);

	hook::Type<IDirect3DDevice9*> Dx9Device = hook::Type<IDirect3DDevice9*>(0x1205750);
	ImGui_ImplDX9_Init(Dx9Device);
}

void ImGuiManager::OnEndScene()
{
	if (ImDrawData* DrawData = ImGui::GetDrawData())
	{
		ImGui_ImplDX9_RenderDrawData(DrawData);
	}
}

bool ImGuiManager::HasCursorControl() const
{
	return bTakeoverCursor;
}

void ImGuiManager::OpenLevelServices()
{
	// apply more events
	LinkMsg(&DefinedEvents::RunningTickEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerAsDriverEnterVehicleEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerAsPassengerEnterVehicleEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerExitVehicleEvent, 0x8000);
}

void ImGuiManager::CloseLevelServices()
{
	// reset any existing state applied to player / ui
	bPlayerGodModeActive = false;
	bPlayerVehicleGodModeActive = false;
	bWantsUISuppressed = false;

	// reset things used by mod menu
	TargetFamily = nullptr;
	InventoryAddItem_SelectedName.clear();
	InventoryAddItem_SelectedGuid = {};

	// remove other events
	UnlinkMsg(&DefinedEvents::RunningTickEvent);
	UnlinkMsg(&DefinedEvents::PlayerAsDriverEnterVehicleEvent);
	UnlinkMsg(&DefinedEvents::PlayerAsPassengerEnterVehicleEvent);
	UnlinkMsg(&DefinedEvents::PlayerExitVehicleEvent);
}

LRESULT ImGuiManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiManager::DrawTab_PlayerSettings()
{
	if (ImGui::BeginTabItem("Player", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
		{
			if (ImGui::CollapsingHeader("Players State", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Toggle settings such as NoClip and GodMode");

				ImGui::Text("Current Controller ID: %u", LocalPlayer->GetControllerID());

				EARS::Modules::PlayerDebugOptions& DebugOptions = *EARS::Modules::PlayerDebugOptions::GetInstance();

				bool bIsFlyActive = DebugOptions.IsInDebugFly();
				if (ImGui::Checkbox("Fly Mode", &bIsFlyActive))
				{
					DebugOptions.SetIsInDebugFly(bIsFlyActive);
				}

				bool bNewGodModeActive = bPlayerGodModeActive;
				if (ImGui::Checkbox("God Mode", &bNewGodModeActive))
				{
					EARS::Modules::StandardDamageComponent* DamageComp = LocalPlayer->GetDamageComponent();
					DamageComp->SetInvincible(bNewGodModeActive);

					bPlayerGodModeActive = bNewGodModeActive;
				}

				if (ImGui::Button("Inspect Player"))
				{
					InitialiseNPCInspector(LocalPlayer, true);
				}

				if (EARS::Modules::CrimeManager* CrimeMgr = EARS::Modules::CrimeManager::GetInstance())
				{
					if (ImGui::Button("Call off the police"))
					{
						CrimeMgr->CalmPoliceTowardsCorleones();
					}
				}
			}

			if (ImGui::CollapsingHeader("Players Inventory", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Modify Players Inventory (Unlimited Ammo, swapping weapons)");

				Mod::ObjectManager& ObjMgrRef = Mod::ObjectManager::GetCheckedRef();

				if (EARS::Modules::InventoryManager* PlayerInventoryMgr = LocalPlayer->GetInventoryManager())
				{
					const char* Label = PlayerInventoryMgr->HasPlayerInfiniteAmmo() ? "Remove Unlimited Ammo" : "Give Unlimited Ammo";
					if (ImGui::Button(Label))
					{
						PlayerInventoryMgr->ToggleUnlimitedAmmo();
					}

					ImGui::PushItemWidth(-1.0f);
					if(ImGui::BeginCombo("##add_to_inventory", InventoryAddItem_SelectedName.data()))
					{
						ObjMgrRef.ForEachItem([&](const std::string& Name, const EARS::Common::guid128_t& EntityID)
						{
								if (ImGui::Selectable(Name.data(), (InventoryAddItem_SelectedGuid == EntityID)))
								{
									InventoryAddItem_SelectedName = Name;
									InventoryAddItem_SelectedGuid = EntityID;
								}
						});

						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();

					if (ImGui::Button("Add To Inventory"))
					{
						if (EARS::Modules::Item* NewItem = PlayerInventoryMgr->TrySpawnItem(InventoryAddItem_SelectedGuid, LocalPlayer->GetStream()))
						{
							const bool bExistingValue = NewItem->GetFanFareWhenAcquiredFlag();
							NewItem->SetFanFareWhenAcquiredFlag(false);
							NewItem->SetForceIntoInventoryFlag(true);
							PlayerInventoryMgr->AddItemToInventory(NewItem, false);
							NewItem->SetFanFareWhenAcquiredFlag(true);
							NewItem->SetForceIntoInventoryFlag(bExistingValue);
						}
					}
				}
			}

			if (ImGui::CollapsingHeader("Players Family", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Modify any characteristics of the Family the Player is part of");

				if (EARS::Modules::Family* PlayersFamily = LocalPlayer->GetFamily())
				{
					static float DesiredMoney = 0.0f;

					// button
					if (ImGui::Button("Modify Balance"))
					{
						PlayersFamily->ModifyBalance(DesiredMoney, EARS::Modules::LedgerItemType::LEDGERITEMTYPE_REVENUE_OTHER);
					}

					ImGui::SameLine();

					// entry box
					ImGui::PushItemWidth(-1.0f);
					ImGui::InputFloat("###modify_balance", &DesiredMoney);
					ImGui::PopItemWidth();
				}
			}
		
			if (ImGui::CollapsingHeader("Players Vehicle", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TextWrapped("Modify any characteristics of the Vehicle the Player occupies");

				if (EARS::Vehicles::WhiteboxCar* CurrentCar = LocalPlayer->GetVehicle())
				{
					ImGui::Text("Current Car: 0x%X", CurrentCar);

					bool bNewVehicleGodModeActive = bPlayerVehicleGodModeActive;
					if (ImGui::Checkbox("Vehicle God Mode", &bNewVehicleGodModeActive))
					{
						SetVehicleGodMode(CurrentCar, bNewVehicleGodModeActive);
						bPlayerVehicleGodModeActive = bNewVehicleGodModeActive;
					}
				}
				else
				{
					ImGui::TextColored({ 255, 0, 0, 255 }, "Player is not in a car, cannot show options");
				}
			}
		}
		else
		{
			ImGui::Text("Local Player is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_CheckpointSettings()
{
	if (ImGui::BeginTabItem("Checkpoints", nullptr, ImGuiTabItemFlags_None))
	{
		EARS::Modules::CheckpointManager* CheckpointMgr = EARS::Modules::CheckpointManager::GetInstance();

		if (ImGui::CollapsingHeader("Current Checkpoint", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// get active and tell user
			if (EARS::Modules::Checkpoint* CurrentCheckpoint = CheckpointMgr->GetCurrentCheckpoint())
			{
				const String& DebugName = CurrentCheckpoint->GetDebugName();
				ImGui::Text("Current Checkpoint: %s", DebugName.c_str());
			}
		}

		// TODO: Consider extending this so we have more info in ImGui such as checkpoint and chapters
		if(ImGui::CollapsingHeader("Select Checkpoint", ImGuiTreeNodeFlags_DefaultOpen))
		{
			CheckpointMgr->ForEachCheckpoint([&](EARS::Modules::Checkpoint& CurCheckpoint) {
				ImGui::PushID(&CurCheckpoint);

				// compile name
				const String& DebugName = CurCheckpoint.GetDebugName();

				// present selectable element which user can press
				ImGui::Bullet();
				if (ImGui::Selectable(DebugName.c_str(), (CheckpointMgr->GetCurrentCheckpoint() == &CurCheckpoint)))
				{
					CheckpointMgr->RestartNewCheckpoint(&CurCheckpoint, EARS::Modules::CheckpointManager::RestartType::RESTART_DEBUG_TELEPORT, 0);
				}

				ImGui::PopID();
				});
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_TimeOfDaySettings()
{
	if (ImGui::BeginTabItem("Time Of Day", nullptr, ImGuiTabItemFlags_None))
	{
		EARS::Modules::TimeOfDayManager* TODManager = EARS::Modules::TimeOfDayManager::GetInstance();
		if (TODManager)
		{
			EARS::Modules::TimeOfDayManager::GameTime CurrentTime = TODManager->GetGameTime();

			ImGui::TextDisabled("Year/Day/Hour/Minute");
			if (ImGui::InputInt4("##time_of_day_input", &CurrentTime.m_Year, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				TODManager->SetGameTime(CurrentTime);
			}
		}
		else
		{
			ImGui::Text("Time Of Day is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_DemographicSettings()
{
#if SHOW_DEMOGRAPHICS_TAB
	if (ImGui::BeginTabItem("Demographic Regions", nullptr, ImGuiTabItemFlags_None))
	{
		EARS::Modules::DemographicRegionManager* DRMgr = EARS::Modules::DemographicRegionManager::GetInstance();
		if (DRMgr)
		{
			ImGui::Text("Current Region: %p", DRMgr->GetCurrentRegion());

			if (ImGui::TreeNode("Registered Regions"))
			{
				DRMgr->ForEachDemographicRegion([](const EARS::Modules::DemographicRegion& InRegion) {
						const std::string RegionName = InRegion.GetDebugName();
						ImGui::Text("%s - (%p)", RegionName.data(), &InRegion);
					});

				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("Demographic Regions Manager is missing!");
		}

		ImGui::EndTabItem();
	}
#endif // SHOW_DEMOGRAPHICS_TAB
}

void ImGuiManager::DrawTab_CitiesSettings()
{
	if (ImGui::BeginTabItem("Cities", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Modules::CityManager* CityMgr = EARS::Modules::CityManager::GetInstance())
		{
			const uint32_t CurrentCityID = CityMgr->GetCurrentCity();
			const String* CurrentCityName = CityMgr->GetDisplayName(CurrentCityID);
			ImGui::Text("Current City: %s", (CurrentCityName ? CurrentCityName->c_str() : "None"));

			if (ImGui::TreeNode("Registered Cities"))
			{
				CityMgr->ForEachCity([](EARS::Modules::City& InCity) {
					if (ImGui::TreeNodeEx((void*)InCity.GetCityID(), ImGuiTreeNodeFlags_DefaultOpen, "%s", InCity.GetDisplayName()->c_str()))
					{
						bool bIsVisible = InCity.IsKnownToPlayer();
						if (ImGui::Checkbox("Is Visible To Player", &bIsVisible))
						{
							if (bIsVisible)
							{
								// Switch to visible
								InCity.RevealToPlayer();
							}
							else
							{
								// switch to hidden
								InCity.HideFromPlayer();
							}
						}

						if (ImGui::Button("Travel To City"))
						{
							InCity.RequestTeleport();
						}

						ImGui::TreePop();
					}
					});

				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("City Manager is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_FamiliesSettings()
{
#if SHOW_FAMILY_TAB
	EARS::Modules::FamilyManager* FamilyMgr = EARS::Modules::FamilyManager::GetInstance();
	if (!FamilyMgr)
	{
		return;
	}

	if (ImGui::BeginTabItem("Families", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::BeginChild("family_contents");

		const char* Preview = "<select_family>";
		if (TargetFamily)
		{
			Preview = TargetFamily->GetInternalName()->c_str();
		}

		ImGui::PushItemWidth(-1.0f);
		if(ImGui::BeginCombo("###select_family", Preview))
		{
			FamilyMgr->ForEachStrategyFamily([&](EARS::Modules::Family& InFamily) 
			{
				const char* FamilyName = InFamily.GetInternalName()->c_str();
				bool bSelected = (TargetFamily == &InFamily);
				if(ImGui::Selectable(FamilyName, &bSelected))
				{
					TargetFamily = &InFamily;
				}
			});

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		if (TargetFamily)
		{
			if (ImGui::CollapsingHeader("Strategy Game", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Compound Venue ID: %u", TargetFamily->GetCompoundVenueID());

				if (ImGui::Button("Revive Family"))
				{
					TargetFamily->ReviveFamily();
				}

				float MinTurnInterval = TargetFamily->GetMinTurnInterval();
				if (ImGui::InputFloat("Min Turn Interval", &MinTurnInterval))
				{
					TargetFamily->SetMinTurnInterval(MinTurnInterval);
				}

				float MaxTurnInterval = TargetFamily->GetMaxTurnInterval();
				if (ImGui::InputFloat("Max Turn Interval", &MaxTurnInterval))
				{
					TargetFamily->SetMaxTurnInterval(MaxTurnInterval);
				}

				float ResponseDelay = TargetFamily->GetResponseDelay();
				if (ImGui::InputFloat("Response Delay", &ResponseDelay))
				{
					TargetFamily->SetResponseDelay(ResponseDelay);
				}

				/*const uint32_t FamilyAlly1 = TargetFamily->GetAllyFamilyID(1);
				ImGui::Text("Ally 1: %u", FamilyAlly1);
				const uint32_t FamilyAlly2 = TargetFamily->GetAllyFamilyID(2);
				ImGui::Text("Ally 2: %u", FamilyAlly2);
				const uint32_t FamilyAlly3 = TargetFamily->GetAllyFamilyID(3);
				ImGui::Text("Ally 3: %u", FamilyAlly3);*/
			}

			if (ImGui::CollapsingHeader("Made Men", ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (uint32_t i = 0; i < TargetFamily->GetNumMadeMen(); i++)
				{
					EARS::Modules::MadeMan* CurMadeMan = TargetFamily->GetMadeManByIndex(i);
					const String* Name = CurMadeMan->GetSimNPC()->GetName();
					if (ImGui::TreeNode(CurMadeMan, "%s", Name->c_str()))
					{
						ImGui::BulletText("State: %s", EARS::Modules::MadeMan::StateEnumToString(CurMadeMan->GetState()));
						ImGui::BulletText("Venue ID: %u", CurMadeMan->GetVenueID());
						ImGui::BulletText("Rank: %u", CurMadeMan->GetRank());
						ImGui::BulletText("State Cooldown: %f", CurMadeMan->GetCountdown());

						switch (CurMadeMan->GetState())
						{
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_HOSPITAL:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_JAIL:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_COOLDOWN:
							{
								if (ImGui::Button("Reset State"))
								{
									CurMadeMan->SendToCompound();
								}
								break;
							}
							case EARS::Modules::MadeManState::MADE_MAN_STATE_ELIMINATED:
							{
								if (ImGui::Button("Revive Made Man"))
								{
									TargetFamily->ReviveMadeMan(*CurMadeMan->GetSimNPC());
								}
								break;
							}
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_COMBAT:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_HIDDEN:
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IN_TRANSIT:
							{
								// TODO: Leave as is?
								break;
							}
							case EARS::Modules::MadeManState::MADE_MAN_STATE_IDLE:
							{
								if (ImGui::Button("Hospitalize"))
								{
									TargetFamily->HospitalizeMadeMan(*CurMadeMan->GetSimNPC());
								}
								ImGui::SameLine();
								if (ImGui::Button("Incarcerate"))
								{
									TargetFamily->IncarcerateMadeMan(*CurMadeMan->GetSimNPC());
								}
								break;
							}
						}

						ImGui::TreePop();

					}
				}
			}

			if (ImGui::CollapsingHeader("Omerta Table", ImGuiTreeNodeFlags_DefaultOpen))
			{
				TargetFamily->ForEachOmertaTable([&](EARS::Modules::Family::OmertaEntry& OmertaEntry)
					{
						const EARS::Modules::Family* TargetFamily = FamilyMgr->GetFamily(OmertaEntry.m_FamilyID);
						const char* FamilyName = TargetFamily->GetInternalName()->c_str();

						ImGui::InputFloat(FamilyName, &OmertaEntry.m_Omerta);
					});
			}
		}

		ImGui::EndChild();

		ImGui::EndTabItem();
	}
#endif // SHOW_FAMILY_TAB
}

void ImGuiManager::DrawTab_PlayerFamilyTreeSettings()
{
	Mod::ObjectManager& ObjMgr = Mod::ObjectManager::GetCheckedRef();

	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();

	if (ImGui::BeginTabItem("Player Family Tree Settings", nullptr, ImGuiTabItemFlags_None))
	{
		EARS::Modules::CorleoneFamilyData* FamilyData = EARS::Modules::CorleoneFamilyData::GetInstance();
		if (!FamilyData)
		{
			ImGui::Text("ERROR: Missing CorleoneFamilyData instance");
		}

		EARS::Modules::PlayerFamilyTree* FamilyTreeData = EARS::Modules::PlayerFamilyTree::GetInstance();
		if (!FamilyTreeData)
		{
			ImGui::Text("ERROR: Missing PlayerFamilyTree instance");
		}

		if (!FamilyData || !FamilyTreeData)
		{
			ImGui::EndTabItem();
			return;
		}


		// let the user toggle pre-order bonuses
		bool bCurrentPreOrderFlag = FamilyData->HasUnlockedPreOrderCrew();
		if (ImGui::Checkbox("Unlock Pre-Order Crew", &bCurrentPreOrderFlag))
		{
			if (bCurrentPreOrderFlag)
			{
				FamilyData->UnlockPreOrderCrew();
			}
			else
			{
				FamilyData->LockPreOrderCrew();
			}
		}

		if (ImGui::CollapsingHeader("Crew Members (simple)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Add all members to crew"))
			{
				FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {
					if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
					{
						if (MadeManNPC->GetIsCrewMember() == false)
						{
							InMember.JoinCrew();
						}
					}
					});
			}

			ImGui::SameLine();

			if (ImGui::Button("Remove all members from crew"))
			{
				FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {
					if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
					{
						if (MadeManNPC->GetIsCrewMember() == true)
						{
							InMember.LeaveCrew();
						}
					}
					});
			}

			ImGui::SameLine();

			if (ImGui::Button("Unlock Full Tree"))
			{
				FamilyTreeData->SetCurrentTreeType(EARS::Modules::PlayerFamilyTree::FamilyTreeType::FAMILYTREE_TYPE_CONSIGLIORE_UNDERBOSS_2CAPOS_4SOLDIERS);
			}
		}

		if (ImGui::CollapsingHeader("Crew Members (detailed)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			uint32_t CurrentIdx = 0;
			FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {

				const char* Name = "[UNKNOWN]";
				if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
				{
					String* NPC_Name = MadeManNPC->GetName();
					Name = NPC_Name->c_str();
				}

				if (ImGui::TreeNode(&InMember, "Member[%u] -> '%s'", CurrentIdx, Name))
				{
					ImGui::Text("SimNPC: %p", InMember.GetSimNPC());
					ImGui::Text("Flags: %u", InMember.GetFlags().GetAllFlags());
					ImGui::Text("Rank: %i", InMember.GetRank());

					const EARS::Common::guid128_t WeaponGUID = InMember.GetWeaponGUID();
					ImGui::Text("Weapon GUID: [%p %p %p %p]", WeaponGUID.a, WeaponGUID.b, WeaponGUID.c, WeaponGUID.d);

					if (EARS::Modules::SimNPC* SimulatedNPC = InMember.GetSimNPC())
					{
						if (ImGui::Button("Toggle Spawn (As Crew Member)"))
						{
							const bool bInCrew = SimulatedNPC->GetIsCrewMember();
							if (SimulatedNPC->GetIsCrewMember())
							{
								InMember.LeaveCrew();
							}
							else
							{
								InMember.JoinCrew();
							}
						}

						if (EARS::Modules::NPC* CrewNPC = SimulatedNPC->GetNPC())
						{
							if (ImGui::Button("Inspect"))
							{
								InitialiseNPCInspector(CrewNPC, false);
							}
						}

						// Provide the option to change weapon license for this character
						const EARS::Common::guid128_t SimNPCID = SimulatedNPC->InqInstanceID();
						uint8_t WeaponLicense = FamilyData->GetWeaponLicense(SimNPCID);
						if (ImGui::SliderScalar("Weapon License", ImGuiDataType_U8, &WeaponLicense, &EARS::Modules::CorleoneFamilyData::MIN_WEAPON_LICENSE, &EARS::Modules::CorleoneFamilyData::MAX_WEAPON_LICENSE))
						{
							FamilyData->SetWeaponLicense(SimNPCID, WeaponLicense);
						}
					}

					if (ImGui::TreeNode("Specialties"))
					{
						auto RenderCheckBox = [&InMember](const std::string& Name, const EARS::Modules::Specialties Index)
							{
								bool bValue = InMember.HasSpecialty(Index);
								if (ImGui::Checkbox(Name.data(), &bValue))
								{
									InMember.ToggleSpecialty(Index);
								}
							};

						RenderCheckBox("Demolitions", EARS::Modules::Specialties::SPECIALITY_DEMO);
						RenderCheckBox("Arsonist", EARS::Modules::Specialties::SPECIALITY_ARSONIST);
						RenderCheckBox("Safecracker", EARS::Modules::Specialties::SPECIALITY_SAFECRACKER);
						RenderCheckBox("Engineer", EARS::Modules::Specialties::SPECIALITY_ENGINEER);
						RenderCheckBox("Medic", EARS::Modules::Specialties::SPECIALITY_MEDIC);
						RenderCheckBox("Bruiser", EARS::Modules::Specialties::SPECIALITY_BRUISER);

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Replace Made Man"))
					{
						Mod::ObjectEntryList& SimNPCList = ObjMgr.GetSimNPCList();
						SimNPCList.DrawList();

						if (ImGui::Button("Replace"))
						{
							const EARS::Common::guid128_t TargetGUID = SimNPCList.GetSelectedGUID();
							if (RWS::CAttributePacket* FoundPacket = SimMgr->GetAttributePacket(&TargetGUID, 0))
							{
								auto PacketIt = FoundPacket->GetEntityIterator();
								RWS::CAttributeHandler* FirstHandler = PacketIt.GetEntity_Mutable();
								EARS::Modules::SimNPC* AsSimNPC = static_cast<EARS::Modules::SimNPC*>(FirstHandler);

								// need to remove prior family member
								auto FoundSlotIndex = FamilyTreeData->FindTreeSlotIndex(InMember.GetSimNPC());
								if (FoundSlotIndex != EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_INVALID)
								{
									FamilyTreeData->RemoveFamilyMember(FoundSlotIndex, true);
								}

								// by default rely on existing member
								// TODO: Fetch from the SimNPC
								uint32_t DesiredSpecialities = InMember.GetSpecialities();
								if (DesiredSpecialities == 0)
								{
									DesiredSpecialities = (uint32_t)EARS::Modules::Specialties::SPECIALITY_ARSONIST;
								}

								// We must release MobFace, otherwise we may risk a crash with non-mobface types.
								EARS::Modules::MobfaceManager& MobFaceMgr = *EARS::Modules::MobfaceManager::GetInstance();
								MobFaceMgr.ResetMobfaceForSlot(FoundSlotIndex);

								// now replace with the new family member
								FamilyTreeData->AddFamilyMember(
									InMember.GetRank(),
									AsSimNPC,
									DesiredSpecialities,
									EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_INVALID,
									nullptr /* weapon guid */);
							}
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}

				CurrentIdx++;

				});
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_UIHUDSettings()
{
	if (ImGui::BeginTabItem("UI Hud", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Apt::UIHUD* UIHudManager = EARS::Apt::UIHUD::GetInstance())
		{
			if (ImGui::Checkbox("Hide HUD", &bWantsUISuppressed))
			{
				if (bWantsUISuppressed)
				{
					UIHudManager->Suppress();
				}
				else
				{
					UIHudManager->Unsuppress();
				}
			}
		}
		else
		{
			ImGui::Text("City Manager is missing!");
		}

		if (EARS::Locale::LocaleManager* LocaleMgr = EARS::Locale::LocaleManager::GetInstance())
		{
			if (ImGui::BeginCombo("###select_language_text", "Select Language"))
			{
				for (uint32_t i = 0; i < LocaleMgr->GetNumLanguages(); i++)
				{
					if (LocaleMgr->GetTextLanguageIsUserSelectable(i))
					{
						const char* LanguageCode = LocaleMgr->GetTextLanguageCode(i);
						const std::string Label = LocaleMgr->GetLanguageName(LanguageCode);

						if (ImGui::Selectable(Label.c_str()))
						{
							LocaleMgr->SetCurrentLanguage(i);
						}
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("###select_language_audio", "Select Audio Language"))
			{
				for (uint32_t i = 0; i < LocaleMgr->GetNumLanguages(); i++)
				{
					if (LocaleMgr->GetTextLanguageIsUserSelectable(i))
					{
						const char* LanguageCode = LocaleMgr->GetTextLanguageCode(i);
						const int AudioIndex = LocaleMgr->FindAudioLanguageIndex(LanguageCode);

						if (AudioIndex != -1)
						{
							const std::string Label = LocaleMgr->GetLanguageName(LanguageCode);

							if (ImGui::Selectable(Label.c_str()))
							{
								LocaleMgr->SetCurrentAudioLanguage(AudioIndex);
							}
						}
					}
				}

				ImGui::EndCombo();
			}
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_ObjectMgrSettings()
{
	if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
	{
		if (ImGui::BeginTabItem("Object Manager"))
		{
			Mod::ObjectManager& ObjMgrRef = Mod::ObjectManager::GetCheckedRef();
			ObjMgrRef.ImGuiDrawContents();

			ImGui::EndTabItem();
		}
	}
}

void ImGuiManager::DrawTab_SimMgrSettings()
{
	if (ImGui::BeginTabItem("Sim Manager"))
	{
		EARS::Framework::SimManager& SimMgr = *EARS::Framework::SimManager::GetInstance();
		EARS::Framework::StreamManager& StreamMgr = *EARS::Framework::StreamManager::GetInstance();

		static RWS::CAttributePacket* FoundPacket = nullptr;

		static EARS::Common::guid128_t PacketGUID;

		ImGui::Text("Search for a Packet:");
		ImGui::InputScalarN("###packet_search", ImGuiDataType_U32, &PacketGUID.a, 4);
		if (ImGui::Button("Find"))
		{
			FoundPacket = SimMgr.GetAttributePacket(&PacketGUID, 0);
		}

		if (FoundPacket)
		{
			const EARS::Common::guid128_t PacketID = FoundPacket->GetInstanceID();
			const uint32_t ClassID = FoundPacket->GetIdOfClassToCreate();
			const uint32_t StreamHdl = FoundPacket->GetStreamHandle();

			EARS::Framework::Stream* Str = StreamMgr.GetStreamFromHandle(StreamHdl);
			const char* StrFilename = Str->GetFileName();

			ImGui::Text("Parent Stream: %s (%u)", StrFilename, StreamHdl);
			ImGui::Value("ClassID", ClassID);
			
			auto EntityIt = FoundPacket->GetEntityIterator();
			while (EntityIt.IsFinished() == false)
			{
				ImGui::Text("%p", EntityIt.GetEntity());

				EARS::Framework::Entity* AsEntity = reinterpret_cast<EARS::Framework::Entity*>(EntityIt.GetEntity_Mutable());

				EntityIt++;
			}

		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_Support()
{
	auto AddUnderLine = [](ImColor col_)
		{
			ImVec2 min = ImGui::GetItemRectMin();
			ImVec2 max = ImGui::GetItemRectMax();
			min.y = max.y;
			ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
		};

	auto TextURL = [&AddUnderLine](const char* Name, const char* URL)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			ImGui::Text(Name);
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseClicked(0))
				{
					ShellExecuteA(0, 0, URL, 0, 0, SW_SHOW);
				}

				AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
				ImGui::SetTooltip("Open in browser\n\t %s", URL);
			}
			else
			{
				AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
			}
		};

	// shamelessly plug donations
	if (ImGui::BeginTabItem("Support", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::TextWrapped("If you want to support the development of this project, please consider donating! Donations with accompanying feature requests will be considered for upcoming versions.");
		TextURL("Patreon", "https://www.patreon.com/Greavesy");
		TextURL("Ko-fi", "https://ko-fi.com/greavesy");
		TextURL("Boosty", "https://boosty.to/greavesy/donate");

		ImGui::EndTabItem();
	}
}

void ImGuiManager::OnTick()
{
	const Settings& OwnSettingsMgr = Settings::GetCheckedRef();
	if (GetAsyncKeyState(OwnSettingsMgr.GetShowModMenuWindowInput()) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		bShowModMenuWindow = !bShowModMenuWindow;
	}

	// Update cursor visibility
	// Should only really be present when any ImGui windows are open - 
	// The ingame cursor (for menus) is expected to be powered by Apt.
	const bool bCursorVisibilityThisFrame = bShowModMenuWindow;
	if (bCursorVisibilityThisFrame != bTakeoverCursor)
	{
		bTakeoverCursor = bCursorVisibilityThisFrame;

		EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();

		if (bTakeoverCursor)
		{
			// DISABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerDisableControlsEventId = hook::Type<RWS::CEventId>(0x112B56C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerDisableControlsEventId, false);

			CameraMgr->DisableUpdate();
		}
		else
		{
			// ENABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerEnableControlsEventId = hook::Type<RWS::CEventId>(0x112B39C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerEnableControlsEventId, false);

			CameraMgr->EnableUpdate();
		}
	}

	ImGuiIO& IO = ImGui::GetIO();
	IO.MouseDrawCursor = bTakeoverCursor;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (bShowModMenuWindow)
	{
		if (ImGui::Begin("Scripthook Menu", &bShowModMenuWindow))
		{
			if (ImGui::BeginTabBar("mod_menu_tab_bar"))
			{
				DrawTab_PlayerSettings();

				DrawTab_CheckpointSettings();

				DrawTab_TimeOfDaySettings();

#if SHOW_DEMOGRAPHICS_TAB
				DrawTab_DemographicSettings();
#endif // SHOW_DEMOGRAPHICS_TAB

				DrawTab_ObjectMgrSettings();

#if SHOW_ATTRIBUTEPACKET_WINDOW
				DrawTab_SimMgrSettings();
#endif // SHOW_ATTRIBUTEPACKET_WINDOW

				DrawTab_CitiesSettings();


#if SHOW_FAMILY_TAB
				DrawTab_FamiliesSettings();
#endif // SHOW_FAMILY_TAB

				DrawTab_PlayerFamilyTreeSettings();

				DrawTab_UIHUDSettings();

				DrawTab_Support();

				ImGui::EndTabBar();
			}

			ImGui::End();
		}

		CurrentInspector.DrawWindow();
	}

	ImGui::EndFrame();
	ImGui::Render();
}

bool ImGuiManager::SetVehicleGodMode(EARS::Vehicles::WhiteboxCar* InVehicle, bool bGodModeActive) const
{
	if (InVehicle)
	{
		EARS::Modules::StandardDamageComponent* DamageComp = InVehicle->GetDamageComponent();
		if (!DamageComp)
		{
			C_Logger::Printf("Missing StandardDamageComponent on %x, cannot apply GodMode!", InVehicle);
			return false;
		}

		// Apply!
		DamageComp->SetInvincible(bGodModeActive);

		return true;
	}

	return false;
}

void ImGuiManager::InitialiseNPCInspector(EARS::Modules::Sentient* InSentient, const bool bIsPlayer)
{
	CurrentInspector.Initialise(InSentient, bIsPlayer);
}
