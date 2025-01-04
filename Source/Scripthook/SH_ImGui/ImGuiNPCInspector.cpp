#include "ImGuiNPCInspector.h"

#include "addons/ImGui/imgui.h"

// SDK
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/Components/NPCUpgradeComponent.h"
#include "SDK/EARS_Godfather/Modules/Components/PlayerUpgradeComponent.h"

void ImGuiNPCInspector::Initialise(EARS::Modules::Sentient* InSentient, const bool bInPlayerObject)
{
	ActiveObject = InSentient;
	bIsPlayer = bInPlayerObject;
}

void ImGuiNPCInspector::DrawWindow()
{
	if (!IsActive())
	{
		// this initialiser is not active
		return;
	}

	if (!ImGui::Begin("NPC Inspector"))
	{
		return;
	}

	if (ImGui::BeginTabBar("npc_tab_bar"))
	{
		DrawTab_GeneralState();

		DrawTab_Appearance();

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void ImGuiNPCInspector::Clear()
{
	ActiveObject = nullptr;
	bIsPlayer = false;
}

void ImGuiNPCInspector::DrawTab_GeneralState()
{
	if (ImGui::BeginTabItem("General", nullptr, ImGuiTabItemFlags_None))
	{
		// draw entity position
		// TODO: Consider bind parents? Vehicles?
		const RwV3d EntityPos = ActiveObject->GetPosition();
		ImGui::Text("Position: %f %f %f", EntityPos.m_X, EntityPos.m_Y, EntityPos.m_Z);

		ImGui::EndTabItem();
	}
}

void ImGuiNPCInspector::DrawTab_Appearance()
{
	if (ImGui::BeginTabItem("Appearance", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::TextWrapped("Adjust Appearance (Such as Bulletproof Vest, Ammo Belt)");

		if (EARS::Modules::UpgradeComponent* UpgradeComp = GetUpgradeComponent())
		{
			if (ImGui::Button("Hide Upgrade Parts"))
			{
				UpgradeComp->ModifyAllUpgradeParts(false);
			}

			if (ImGui::Button("Show Upgrade Parts"))
			{
				UpgradeComp->ModifyAllUpgradeParts(true);
			}
		}

		ImGui::EndTabItem();
	}
}

EARS::Modules::UpgradeComponent* ImGuiNPCInspector::GetUpgradeComponent() const
{
	if (bIsPlayer)
	{
		EARS::Modules::Player* AsPlayer = EARS::Framework::_QueryInterface<EARS::Modules::Player>(ActiveObject, 0x383225A1);
		return AsPlayer->GetUpgradeComponent();
	}
	else
	{
		EARS::Modules::NPC* AsNPC = EARS::Framework::_QueryInterface<EARS::Modules::NPC>(ActiveObject, 0xA0D329D6);
		return AsNPC->GetUpgradeComponent();
	}
}
