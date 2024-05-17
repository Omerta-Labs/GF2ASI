#include "ImGuiManager.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/PartedModel/PartedModelMgr.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

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
	ShowPartedModelWindowInput = GetPrivateProfileInt("Keybinds", "model", VK_F1, "./gf2asi.ini");
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

void ImGuiManager::OnTick()
{
	if (GetAsyncKeyState(ShowPartedModelWindowInput) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		bShowPartedModelWindow = !bShowPartedModelWindow;
	}

	if (GetAsyncKeyState(ShowImGuiDemoWindowInput) & 1) //ImGui::IsKeyPressed(ImGuiKey_F2)
	{
		bShowImGuiDemoWindow = !bShowImGuiDemoWindow;
	}

	// Update cursor visibility
	// Should only really be present when any ImGui windows are open - 
	// The ingame cursor (for menus) is expected to be powered by Apt.
	const bool bCursorVisibilityThisFrame = bShowImGuiDemoWindow || bShowPartedModelWindow;
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

	if (bShowPartedModelWindow)
	{
		PartedModelMgr* ModelMgr = PartedModelMgr::GetInstance();
		if (ModelMgr == nullptr)
		{
			// not loaded yet so don't show anything
			return;
		}

		if (ImGui::Begin("Parted Model Manager", &bShowPartedModelWindow))
		{
			ImGui::Text("Parted Model List");
			ImGui::BeginChild("parted_model_list");

			PartedModel::AssemblyListHeader* CurrentAssembly = ModelMgr->m_AssembliesList;
			while (CurrentAssembly != nullptr)
			{
				for (uint32_t i = 0; i < CurrentAssembly->m_NumAssemblies; i++)
				{
					const PartedModel::Assembly& ThisAssembly = CurrentAssembly->m_Assemblies[i];
					if (ImGui::TreeNodeEx(&ThisAssembly, ImGuiTreeNodeFlags_None, "%s [%s]", ThisAssembly.m_AssemblyName, (ThisAssembly.m_Flags == 4 ? "VEHICLE" : "CHARACTER")))
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
							ImGui::Text("%s", ThisPreset.m_PresetName);
							ImGui::PopID();
						}

						ImGui::TreePop();
					}
				}

				CurrentAssembly = CurrentAssembly->m_NextHeader;
			}

			ImGui::EndChild();
			ImGui::End();
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
}
