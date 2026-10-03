#include "ImGuiManager.h"

#include "Addons/Hook.h"
#include "Addons/Settings.h"
#include "Addons/imgui/imgui.h"
#include "Scripthook/SH_ImGui/GameEventIds.h"
#include "Addons/KeybindManager.h"
#include "Scripthook/SH_ObjectManager/ObjectManager.h"

// SDK
#include "framework/core/graphics/materialhash.h"
#include "framework/core/graphics/materialmanager.h"
#include "framework/core/simmanager/simmanager.h"
#include "framework/core/streammanager/streammanager.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/group/groupmanager.h"
#include "modules/buildings/building.h"
#include "modules/buildings/building_manager.h"
#include "modules/buildings/building_store.h"
#include "modules/families/corleone_data.h"
#include "modules/families/family.h"
#include "modules/families/family_manager.h"
#include "modules/families/mademan.h"
#include "modules/item/inventorymanager.h"
#include "modules/mobface/mobfacemanager.h"
#include "modules/npc/crime/crimemanager.h"
#include "modules/npc/npc.h"
#include "modules/npcscheduling/demographicregion.h"
#include "modules/npcscheduling/demographicregionmanager.h"
#include "modules/npcscheduling/simnpc.h"
#include "modules/player/player.h"
#include "modules/player/playerdebug.h"
#include "modules/timeofday/timeofdaymanager.h"
#include "modules/turf/city.h"
#include "modules/turf/citymanager.h"
#include "modules/vehicles/behaviors/whitebox_car/whitebox_car.h"
#include "modules/vehicles/vehicledamagecomponent.h"
#include "ears_physics/characters/characterproxy.h"
#include "ears_physics/vehicles/ground/wheeled/havok_wheeled_vehicle.h"
#include "ears_rt_llrender/shadermanager.h"

// C++
#include <filesystem>
#include <string>

void ImGuiManager::SetPlayerGodMode(EARS::Modules::Player& InPlayer) const
{
	EARS::Modules::StandardDamageComponent* DamageComp = InPlayer.GetDamageComponent();
	DamageComp->SetInvincible(bPlayerGodModeActive);
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
					bPlayerGodModeActive = bNewGodModeActive;
					SetPlayerGodMode(*LocalPlayer);
				}

				bool bNewFreezeGameLogic = bFreezeLogic;
				if (ImGui::Checkbox("Freeze Game Logic", &bNewFreezeGameLogic))
				{
					ToggleFreezeLogic();
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
				ImGui::TextWrapped("Modify Players Inventory (Unlimited Ammo, giving weapons has been moved to ObjectManager)");

				Mod::ObjectManager& ObjMgrRef = Mod::ObjectManager::GetCheckedRef();

				if (EARS::Modules::InventoryManager* PlayerInventoryMgr = LocalPlayer->GetInventoryManager())
				{
					const char* Label = PlayerInventoryMgr->HasPlayerInfiniteAmmo() ? "Remove Unlimited Ammo" : "Give Unlimited Ammo";
					if (ImGui::Button(Label))
					{
						PlayerInventoryMgr->ToggleUnlimitedAmmo();
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
		GetCheckpointDebug().DisplayTab();
		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_PhotoMode()
{
	if (ImGui::BeginTabItem("Photo Mode", nullptr, ImGuiTabItemFlags_None))
	{
		PhotoModeSystem.DrawTab();
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

void ImGuiManager::DrawTab_BuildingSettings()
{
	EARS::Modules::FamilyManager* FamilyMgr = EARS::Modules::FamilyManager::GetInstance();
	EARS::Modules::BuildingManager* BuildingMgr = EARS::Modules::BuildingManager::GetInstance();
	EARS::Modules::CityManager* CityMgr = EARS::Modules::CityManager::GetInstance();
	if (!BuildingMgr || !FamilyMgr || !CityMgr)
	{
		return;
	}

	// Utility which implements Family takeover for a single building
	auto DrawFamilyComboBox = [&](EARS::Modules::BuildingStore& Store)
		{
			const EARS::Modules::Family* OwningFamily = FamilyMgr->GetFamily(Store.GetFamilyID());
			const String* FamilyString = OwningFamily->GetInternalName();

			ImGui::PushItemWidth(-1.0f);
			if (ImGui::BeginCombo("###family_selector", FamilyString->c_str()))
			{
				FamilyMgr->ForEachStrategyFamily([&](const EARS::Modules::Family& StrategyFamily)
					{
						const String* SelectableName = StrategyFamily.GetInternalName();
						if (ImGui::Selectable(SelectableName->c_str(), StrategyFamily.GetFamilyID() == Store.GetFamilyID()))
						{
							Store.ChangeOwnership(StrategyFamily.GetFamilyID(), false, nullptr, false);
						}
					});

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		};

	auto DrawPlayerTeleportButton = [&](EARS::Modules::BuildingStore& Store)
		{
			ImGui::PushItemWidth(-1.0f);
			if (ImGui::Button("Teleport"))
			{
				DeferredTeleportPayload = { .TeleportLocation = Store.GetEntrancePos() };

				if (CityMgr->GetCurrentCity() != Store.GetCityID())
				{
					// we need to first teleport to new city to get world partitions sync'd up
					CityMgr->TeleportToCity(Store.GetCityID());

					LinkMsg(&DefinedEvents::iMsgPlayerTeleportDoneExceptFade, 0x8000);
				}
				else
				{
					ProcessBuildingTeleport();
				}
			}
			ImGui::PopItemWidth();
		};

	if (ImGui::BeginTabItem("Buildings", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::BeginChild("building_store_table");
		if (ImGui::BeginTable("active_building_table", 5, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
		{
			const float AvailWidth = ImGui::GetContentRegionAvail().x;
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.08f);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.42f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.1f);
			ImGui::TableSetupColumn("Family", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.2f);
			ImGui::TableSetupColumn("Teleport", ImGuiTableColumnFlags_WidthFixed, AvailWidth * 0.2f);
			ImGui::TableHeadersRow();

			BuildingMgr->ForEachBuildingStore([&](EARS::Modules::BuildingStore& ActiveStore)
				{
					ImGui::PushID(&ActiveStore);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					const String* BuildingString = ActiveStore.GetDisplayName();

					const EARS::Modules::Family* OwningFamily = FamilyMgr->GetFamily(ActiveStore.GetFamilyID());
					const String* FamilyString = OwningFamily->GetInternalName();

					ImGui::Text("%u", ActiveStore.GetVenueID());
					ImGui::TableNextColumn();
					ImGui::Text("%s", BuildingString->c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", BuildingMgr->GetBuildingTypeInternalName(ActiveStore.GetBuildingType()));
					ImGui::TableNextColumn();
					DrawFamilyComboBox(ActiveStore);
					ImGui::TableNextColumn();
					DrawPlayerTeleportButton(ActiveStore);
					ImGui::TableNextColumn();

					ImGui::PopID();
				});


			ImGui::EndTable();
		}
		ImGui::EndChild();

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

				ImGui::BeginDisabled(!TargetFamily->HasBeenEliminated());
				if (ImGui::Button("Revive Family"))
				{
					TargetFamily->ReviveFamily();
				}
				ImGui::EndDisabled();

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
								if (ImGui::Button("Send To Component"))
								{
									CurMadeMan->SendToCompound();
								}
								ImGui::SameLine();
								if (ImGui::Button("Eliminate"))
								{
									TargetFamily->KillMadeMan(*CurMadeMan->GetSimNPC());
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
								ImGui::SameLine();
								if (ImGui::Button("Eliminate"))
								{
									TargetFamily->KillMadeMan(*CurMadeMan->GetSimNPC());
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
		ImGui::BeginChild("family_tree_settings_window");

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

		ImGui::EndChild();
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

#if DEBUG
		ImGui::BeginChild("MaterialList");

		static ImGuiTextFilter MaterialSearchFilter;
		MaterialSearchFilter.Draw();

		const uint32_t GeneratedHash = MemUtils::CallCdeclMethod<uint32_t, const char*>(0x60D740, MaterialSearchFilter.InputBuf);

		const static const char* PassNames[5] = 
		{
			"MAT_PASS_SHADOW",
			"MAT_PASS_DEPTH",
			"MAT_PASS_COLOUR",
			"MAT_PASS_DEPTH_LOW",
			"MAT_PASS_COLOUR_LOW"
		};

		MaterialManager::ForEachMaterial([&](Material& CurrentMat)
			{
				if (GeneratedHash != 0 && GeneratedHash == CurrentMat.m_NameHash)
				{
					ImGui::Value("Hash", CurrentMat.m_NameHash);

					ImGui::PushID(&CurrentMat);
					for (uint32_t PassID = 0; PassID < 5; PassID++)
					{
						ImGui::SeparatorText(PassNames[PassID]);
						ImGui::PushID(PassID);

						const uint32_t StartIndex = CurrentMat.m_PassStart[PassID];
						const uint32_t EndIndex = CurrentMat.m_PassEnd[PassID];
						for (uint32_t ParamIdx = StartIndex; ParamIdx < EndIndex; ParamIdx++)
						{
							MatParam& CurrentParam = CurrentMat.m_ParamData[ParamIdx];
							ImGui::Text("Name: 0x%X", CurrentParam.m_ParamNameHash);

							ImGui::PushID(&CurrentParam);
							switch (CurrentParam.m_ParamType)
							{
								case MAT_PARAM_FLOAT1:
								{
									ImGui::InputScalarN("##value", ImGuiDataType_Float, &CurrentParam.m_ParamValue.value, 1);
									break;
								}
								case MAT_PARAM_FLOAT2:
								{
									ImGui::InputScalarN("##value", ImGuiDataType_Float, CurrentParam.m_ParamValue.pValues, 2);
									break;
								}
								case MAT_PARAM_FLOAT3:
								{
									ImGui::InputScalarN("##value", ImGuiDataType_Float, CurrentParam.m_ParamValue.pValues, 3);
									break;
								}
								case MAT_PARAM_FLOAT4:
								{
									ImGui::InputScalarN("##value", ImGuiDataType_Float, CurrentParam.m_ParamValue.pValues, 4);
									break;
								}
								case MAT_PARAM_TEX:
								{
									ImGui::Value("Texture Unfound", CurrentParam.m_ParamValue.textureRef);
									break;
								}
								case MAT_PARAM_TEX_FOUND:
								{
									ImGui::InputScalarN("##value", ImGuiDataType_S32, &CurrentParam.m_ParamValue.textureRef, 1);
									break;
								}
							}
							ImGui::PopID();
						}
						ImGui::PopID();
					}
					ImGui::PopID();

				}
				else if(GeneratedHash == 0)
				{
					ImGui::Value("Hash", CurrentMat.m_NameHash);
				}
			});
		ImGui::EndChild();
#endif // DEBUG

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_Keybinds()
{
	if (!ImGui::BeginTabItem("Keybinds", nullptr, ImGuiTabItemFlags_None))
	{
		return;
	}

	SH::KeybindManager& Keybinds = SH::KeybindManager::GetCheckedRef();

	ImGui::TextWrapped("Bind keyboard shortcuts to menu actions. Click Rebind then press a key; "
		"values are stored as Windows virtual-key codes in gf2asi_keybinds.ini.");

	if (Keybinds.IsRebinding())
	{
		ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Press a key to bind, or Escape to cancel...");
	}
	else
	{
		// Keep the layout stable whether or not a capture is in progress
		ImGui::TextDisabled("Ready.");
	}

	const ImGuiTableFlags TableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp;
	if (ImGui::BeginTable("KeybindTable", 4, TableFlags))
	{
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("##Buttons", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (const SH::ShortcutAction& Action : Keybinds.GetActions())
		{
			ImGui::TableNextRow();
			ImGui::PushID(Action.Id.c_str());

			// Action name, with the category and id available on hover
			ImGui::TableNextColumn();
			ImGui::TextUnformatted(Action.DisplayName.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s  (%s)", Action.Id.c_str(), Action.Category.c_str());
			}

			// Live toggle state, for actions that report it
			ImGui::TableNextColumn();
			if (Action.IsActive)
			{
				const bool bActive = Action.IsActive();
				ImGui::TextColored(bActive ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f) : ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
					bActive ? "ON" : "off");
			}
			else
			{
				ImGui::TextDisabled("-");
			}

			// Current binding
			ImGui::TableNextColumn();
			const bool bCapturingThis = Keybinds.IsRebinding() && Keybinds.GetRebindTargetId() == Action.Id;
			const int VirtualKey = Keybinds.GetBinding(Action.Id);
			if (bCapturingThis)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "[ ... ]");
			}
			else if (VirtualKey == 0)
			{
				ImGui::TextDisabled("Unbound");
			}
			else
			{
				const std::string KeyName = SH::KeybindManager::GetKeyDisplayName(VirtualKey);
				ImGui::TextUnformatted(KeyName.c_str());
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Virtual-key 0x%02X", VirtualKey);
				}
			}

			// Rebind / clear controls, locked out while another capture is running
			ImGui::TableNextColumn();
			ImGui::BeginDisabled(Keybinds.IsRebinding());
			if (ImGui::SmallButton("Rebind"))
			{
				Keybinds.BeginRebind(Action.Id);
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("Clear"))
			{
				Keybinds.SetBinding(Action.Id, 0);
			}
			ImGui::EndDisabled();

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	ImGui::EndTabItem();
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

#if DEBUG
		if(ImGui::Button("Show ImGui Style Editor"))
		{
			bShowImGuiStyleEditor = true;
		}
#endif // DEBUG

		ImGui::EndTabItem();
	}
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

void ImGuiManager::SetPlayerFlyMode(bool bIsActive)
{
}

void ImGuiManager::ToggleFreezeLogic()
{
	bFreezeLogic = !bFreezeLogic;
	if (bFreezeLogic)
	{
		RWS::MainLoop::Logic::PushPause(16);
	}
	else
	{
		RWS::MainLoop::Logic::PopPause(16);
	}
}

void ImGuiManager::InitialiseNPCInspector(EARS::Modules::Sentient* InSentient, const bool bIsPlayer)
{
	CurrentInspector.Initialise(InSentient, bIsPlayer);
}

void ImGuiManager::ProcessBuildingTeleport()
{
	if (DeferredTeleportPayload.has_value())
	{
		const BuildingTeleportPayload& Payload = DeferredTeleportPayload.value();

		RwMatrixTag Transform;
		Transform.m_Pos = Payload.TeleportLocation;

		EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer();
		LocalPlayer->Teleport(Transform, 1152, nullptr, nullptr);

		DeferredTeleportPayload = {};
	}
}
