#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/PartedModel/PartedModelMgr.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"

ImGuiManager::ImGuiManager()
	: CEventHandler()
{

}

void ImGuiManager::HandleEvents(const RWS::CMsg& MsgEvent)
{
	hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	if (MsgEvent.IsEvent(RunningTickEvent))
	{
		OnTick();
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

	// TODO: When the scripthook has an actual setting system, this should fetch from that manager
	ShowModMenuWindowInput = GetPrivateProfileInt("Keybinds", "model", VK_F1, "./gf2asi.ini");
	ShowImGuiDemoWindowInput = GetPrivateProfileInt("Keybinds", "demo", VK_F2, "./gf2asi.ini");
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
								LocalPlayer->TrySwapPlayerModel(ThisAssembly.m_AssemblyName, ThisPreset.m_PresetName);
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
		EARS::Modules::Player* LocalPlayer = EARS::Modules::Player::GetLocalPlayer();
		if (LocalPlayer)
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
		}
		else
		{
			ImGui::Text("Local Player is missing!");
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::OnTick()
{
	if (GetAsyncKeyState(ShowModMenuWindowInput) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		bShowModMenuWindow = !bShowModMenuWindow;
	}

	if (GetAsyncKeyState(ShowImGuiDemoWindowInput) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		bShowImGuiDemoWindow = !bShowImGuiDemoWindow;
	}

	// Update cursor visibility
	// Should only really be present when any ImGui windows are open - 
	// The ingame cursor (for menus) is expected to be powered by Apt.
	const bool bCursorVisibilityThisFrame = bShowImGuiDemoWindow || bShowModMenuWindow;
	if (bCursorVisibilityThisFrame != bTakeoverCursor)
	{
		bTakeoverCursor = bCursorVisibilityThisFrame;

		if (bTakeoverCursor)
		{
			// DISABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerDisableControlsEventId = hook::Type<RWS::CEventId>(0x112B56C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerDisableControlsEventId, false);
		}
		else
		{
			// ENABLE CONTROLS
			hook::Type<RWS::CEventId> PlayerEnableControlsEventId = hook::Type<RWS::CEventId>(0x112B39C);
			MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, PlayerEnableControlsEventId, false);
		}
	}

	ImGuiIO& IO = ImGui::GetIO();
	IO.MouseDrawCursor = bTakeoverCursor;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (bShowImGuiDemoWindow)
	{
		ImGui::ShowDemoWindow(&bShowImGuiDemoWindow);
	}

	if (bShowModMenuWindow)
	{
		if (ImGui::Begin("Scripthook Menu", &bShowModMenuWindow))
		{
			if (ImGui::BeginTabBar("mod_menu_tab_bar"))
			{
				DrawTab_PlayerModelSwap();

				DrawTab_PlayerSettings();

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
}
