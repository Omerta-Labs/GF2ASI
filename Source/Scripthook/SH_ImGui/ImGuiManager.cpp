#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/tLog.h"
#include "Addons/Settings.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"

// Godfather
#include "SDK/EARS_Framework/Game_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Game_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Godfather/Modules/Components/PlayerUpgradeComponent.h"
#include "SDK/EARS_Godfather/Modules/Components/Damage/StandardDamageComponent.h"
#include "SDK/EARS_Godfather/Modules/Families/Family.h"
#include "SDK/EARS_Godfather/Modules/Families/FamilyManager.h"
#include "SDK/EARS_Godfather/Modules/Families/CorleoneData.h"
#include "SDK/EARS_Godfather/Modules/FamilyTree/PlayerFamilyTree.h"
#include "SDK/EARS_Godfather/Modules/Item/Inventory.h"
#include "SDK/EARS_Godfather/Modules/Item/InventoryManager.h"
#include "SDK/EARS_Godfather/Modules/Item/Item.h"
#include "SDK/EARS_Godfather/Modules/PartedModel/PartedModelMgr.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/TimeOfDay/TimeOfDayManager.h"
#include "SDK/EARS_Godfather/Modules/Turf/City.h"
#include "SDK/EARS_Godfather/Modules/Turf/CityManager.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegion.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/DemographicRegionManager.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/UI/UIHud.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/Behaviours/WhiteboxCar/WhiteboxCar.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/VehicleDamageComponent.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"

// CPP
#include <string>

#define ENABLE_ENTITY_SPAWN_DEBUG 0

#if DEBUG
#define SHOW_DEMOGRAPHICS_TAB 1
#else
#define SHOW_DEMOGRAPHICS_TAB 0
#endif // DEBUG

#if ENABLE_ENTITY_SPAWN_DEBUG
class CAttributePacket : DoubleLinkedListNodeMixin2<CAttributePacket>
{
public:

	inline uint32_t GetStreamHandle() const { return m_hStream; }

private:

	unsigned int m_hStream;
	CAttributePacket* m_pPrevSibling;
	CAttributePacket* m_pNextSibling;
	void* m_EntityList = nullptr;
	CAttributePacket* m_pHashNext;
	unsigned __int8 m_flags;
	unsigned __int8 m_pad[3];
	//RWS::__Internal::CAttributeDataChunk firstChunk_;
};

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

class SimManager
{
public:

	CAttributePacket* GetAttributePacket(const EARS::Common::guid128_t* InGuid, int bMaskStream)
	{
		return MemUtils::CallClassMethod<CAttributePacket*, SimManager*, const EARS::Common::guid128_t*, int>(
			0x04461C0, this, InGuid, bMaskStream);
	}

	void* SpawnEntity(const EARS::Common::guid128_t* InGuid, int SpawnFlags)
	{
		return MemUtils::CallClassMethod<void*, SimManager*, const EARS::Common::guid128_t*, int>(
			0x0446130, this, InGuid, SpawnFlags);
	}

	void* SpawnEntity(CAttributePacket* Packet, uint32_t StreamHandle, bool bSkipPostInit)
	{
		return MemUtils::CallClassMethod<void*, SimManager*, CAttributePacket*, uint32_t, bool>(
			0x0446340, this, Packet, StreamHandle, bSkipPostInit);		
	}

	static SimManager* GetInstance()
	{
		// 
		return *(SimManager**)0x1223410;
	}
};
#endif // ENABLE_ENTITY_SPAWN_DEBUG

Settings OurSettings;

namespace DefinedEvents
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	hook::Type<RWS::CEventId> PlayerAsDriverEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E030);
	hook::Type<RWS::CEventId> PlayerAsPassengerEnterVehicleEvent = hook::Type<RWS::CEventId>(0x112E11C);
	hook::Type<RWS::CEventId> PlayerExitVehicleEvent = hook::Type<RWS::CEventId>(0x112E018);
}

ImGuiManager::ImGuiManager()
	: CEventHandler()
{
	LinkMsg(&DefinedEvents::RunningTickEvent, 0x8000);
}

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX9_Shutdown();

	UnlinkMsg(&DefinedEvents::RunningTickEvent);
	UnlinkMsg(&DefinedEvents::PlayerAsDriverEnterVehicleEvent);
	UnlinkMsg(&DefinedEvents::PlayerAsPassengerEnterVehicleEvent);
	UnlinkMsg(&DefinedEvents::PlayerExitVehicleEvent);
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

	OurSettings.Init();

	// apply more events
	LinkMsg(&DefinedEvents::PlayerAsDriverEnterVehicleEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerAsPassengerEnterVehicleEvent, 0x8000);
	LinkMsg(&DefinedEvents::PlayerExitVehicleEvent, 0x8000);
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

LRESULT ImGuiManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiManager::DrawTab_PlayerModelSwap()
{
	if (ImGui::BeginTabItem("Player Model Swap", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::BeginChild("parted_model_list");

		PartedModelMgr* ModelMgr = PartedModelMgr::GetInstance();
		if (ModelMgr == nullptr)
		{
			// not loaded yet so don't show anything
			ImGui::Text("Parted Model Manager is not active, cannot switch models!");
			return;
		}

		ImGui::Text("Find a Preset and press 'SWITCH' to swap Player Models.");

		ModelMgr->ForEachAssemblyHeader([](const PartedModel::AssemblyListHeader& InAssemblyHeader) {

			// First iterate through the assemblies within this header
			for (uint32_t i = 0; i < InAssemblyHeader.m_NumAssemblies; i++)
			{
				const PartedModel::Assembly& ThisAssembly = InAssemblyHeader.m_Assemblies[i];
				if (ThisAssembly.m_Flags == 4)
				{
					// we do not want people turning into cars
					continue;
				}

				// Present the tree node for the presets within the assembly
				if (ImGui::TreeNodeEx(&ThisAssembly, ImGuiTreeNodeFlags_None, "%s", ThisAssembly.m_AssemblyName))
				{
					for (uint32_t x = 0; x < ThisAssembly.m_NumPresets; x++)
					{
						const PartedModel::Preset& ThisPreset = ThisAssembly.m_PresetsArr[x];
						ImGui::PushID(&ThisPreset);
						if (ImGui::Button("SWTICH"))
						{
							if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
							{
								LocalPlayer->TrySwapModel(ThisAssembly.m_AssemblyName, ThisPreset.m_PresetName);
							}
						}
						ImGui::SameLine();
						ImGui::Text(ThisPreset.m_PresetName);
						ImGui::PopID();
					}

					ImGui::TreePop();
				}
			}
			});

		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}

void ImGuiManager::DrawTab_PlayerSettings()
{
	if (ImGui::BeginTabItem("Player", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
		{
			bool bNewFlyModeState = bPlayerFlyModeActive;
			if (ImGui::Checkbox("Fly Mode", &bNewFlyModeState))
			{
				// only change state if updated
				if (bPlayerFlyModeActive != bNewFlyModeState)
				{
					if (bNewFlyModeState)
					{
						EARS::Havok::CharacterProxy* PlayerProxy = LocalPlayer->GetCharacterProxy();
						PlayerProxy->EnableGravity(false);
						PlayerProxy->SetCollisionState(EARS::Havok::CharacterProxy::CollisionState::CS_ALL_DISABLED);
					}
					else
					{
						EARS::Havok::CharacterProxy* PlayerProxy = LocalPlayer->GetCharacterProxy();
						PlayerProxy->EnableGravity(true);
						PlayerProxy->SetCollisionState(EARS::Havok::CharacterProxy::CollisionState::CS_ENABLED);
					}

					bPlayerFlyModeActive = bNewFlyModeState;
				}
			}

			bool bNewGodModeActive = bPlayerGodModeActive;
			if (ImGui::Checkbox("God Mode", &bNewGodModeActive))
			{
				EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer();
				EARS::Modules::StandardDamageComponent* DamageComp = LocalPlayer->GetDamageComponent();
				DamageComp->SetInvincible(bNewGodModeActive);

				bPlayerGodModeActive = bNewGodModeActive;
			}

			if (EARS::Modules::PlayerUpgradeComponent* PlayerUpgradeComp = LocalPlayer->GetUpgradeComponent())
			{
				if (ImGui::Button("Hide Upgrade Parts"))
				{
					PlayerUpgradeComp->ModifyAllUpgradeParts(false);
				}

				if (ImGui::Button("Show Upgrade Parts"))
				{
					PlayerUpgradeComp->ModifyAllUpgradeParts(true);
				}
			}

			if (EARS::Modules::InventoryManager* PlayerInventoryMgr = LocalPlayer->GetInventoryManager())
			{
				const char* Label = PlayerInventoryMgr->HasPlayerInfiniteAmmo() ? "Remove Unlimited Ammo" : "Give Unlimited Ammo";
				if (ImGui::Button(Label))
				{
					PlayerInventoryMgr->ToggleUnlimitedAmmo();
				}
			}

			EARS::Vehicles::WhiteboxCar* CurrentCar = LocalPlayer->GetVehicle();
			if (CurrentCar)
			{
				ImGui::Text("Current Car: 0x%X", CurrentCar);

				bool bNewVehicleGodModeActive = bPlayerVehicleGodModeActive;
				if (ImGui::Checkbox("Vehicle God Mode", &bNewVehicleGodModeActive))
				{
					SetVehicleGodMode(CurrentCar, bNewVehicleGodModeActive);
					bPlayerVehicleGodModeActive = bNewVehicleGodModeActive;
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
						ImGui::Text("%p", &InRegion);
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
			ImGui::Text("Current City: %s", (CurrentCityName ? CurrentCityName->m_pCStr : "None"));

			if (ImGui::TreeNode("Registered Cities"))
			{
				CityMgr->ForEachCity([](EARS::Modules::City& InCity) {
					if (ImGui::TreeNodeEx((void*)InCity.GetCityID(), ImGuiTreeNodeFlags_DefaultOpen, "%s", InCity.GetDisplayName()->m_pCStr))
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

void ImGuiManager::DrawTab_PlayerFamilyTreeSettings()
{
	if (ImGui::BeginTabItem("Player Family Tree Settings", nullptr, ImGuiTabItemFlags_None))
	{
		if (EARS::Modules::PlayerFamilyTree* FamilyTreeData = EARS::Modules::PlayerFamilyTree::GetInstance())
		{
			if (ImGui::TreeNode("FamilyTree Data"))
			{
				uint32_t CurrentIdx = 0;
				FamilyTreeData->ForEachMember([&](EARS::Modules::PlayerFamilyMember& InMember) {

					const char* Name = "Unknown";
					if (EARS::Modules::SimNPC* MadeManNPC = InMember.GetSimNPC())
					{
						String* NPC_Name = MadeManNPC->GetName();
						Name = NPC_Name->m_pCStr;
					}

					if (ImGui::TreeNode(&InMember, "Member[%u] -> '%s'", CurrentIdx, Name))
					{
						ImGui::Text("SimNPC: %p", InMember.GetSimNPC());
						ImGui::Text("Flags: %u", InMember.GetFlags().GetAllFlags());
						ImGui::Text("Rank: %i", InMember.GetRank());

						const EARS::Common::guid128_t WeaponGUID = InMember.GetWeaponGUID();
						ImGui::Text("Weapon GUID: [%p %p %p %p]", WeaponGUID.a, WeaponGUID.b, WeaponGUID.c, WeaponGUID.d);

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


						
						ImGui::TreePop();
					}

					CurrentIdx++;

					});

				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("PlayerFamilyTree is missing!");
		}

		if (EARS::Modules::CorleoneFamilyData* FamilyData = EARS::Modules::CorleoneFamilyData::GetInstance())
		{
			if (ImGui::TreeNode("Honour Data"))
			{
				FamilyData->ForEachHonourData([](EARS::Modules::CorleoneFamilyData::HonorData& InHonourData) {
					if (ImGui::TreeNodeEx(&InHonourData, ImGuiTreeNodeFlags_None, "[0x%X 0x%X 0x%X 0x%X]", InHonourData.m_SimNpcGuid.a, InHonourData.m_SimNpcGuid.b, InHonourData.m_SimNpcGuid.c, InHonourData.m_SimNpcGuid.d))
					{
						// Weapon license, must ensure it is restricted between 1 and 4
						ImGui::SliderScalar("Weapon License", ImGuiDataType_U8, &InHonourData.m_WeaponLicenseLevel, &EARS::Modules::CorleoneFamilyData::MIN_WEAPON_LICENSE, &EARS::Modules::CorleoneFamilyData::MAX_WEAPON_LICENSE);

						ImGui::TreePop();
					}
					});

				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Text("CorleoneFamilyData is missing!");
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
	if (GetAsyncKeyState(OurSettings.GetShowModMenuWindowInput()) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		bShowModMenuWindow = !bShowModMenuWindow;
	}

	if (bPlayerFlyModeActive)
	{
		if (EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer())
		{
			if (GetAsyncKeyState(OurSettings.GetFlyModeUpInput()) & 1)
			{
				LocalPlayer->Translate(0.0f, 10.0f, 0.0f);
			}

			if (GetAsyncKeyState(OurSettings.GetFlyModeDownInput()) & 1)
			{
				LocalPlayer->Translate(0.0f, -10.0f, 0.0f);
			}
		}
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
				DrawTab_PlayerModelSwap();

				DrawTab_PlayerSettings();

				DrawTab_TimeOfDaySettings();

				DrawTab_DemographicSettings();

				DrawTab_CitiesSettings();

				DrawTab_PlayerFamilyTreeSettings();

				DrawTab_UIHUDSettings();

				DrawTab_Support();

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
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