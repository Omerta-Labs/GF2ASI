#include "ImGuiNPCInspector.h"

#include "addons/ImGui/imgui.h"

// SDK
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/NPCScheduling/SimNPC.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/Components/NPCUpgradeComponent.h"
#include "SDK/EARS_Godfather/Modules/Components/PlayerUpgradeComponent.h"
#include "SDK/EARS_Godfather/Modules/Mobface/MobfaceManager.h"
#include "SDK/EARS_Godfather/Modules/PartedModel/PartedModelMgr.h"

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

		DrawTab_Upgrades();

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
		EARS::Modules::PartedModelMgr* ModelMgr = EARS::Modules::PartedModelMgr::GetInstance();
		if (ModelMgr == nullptr)
		{
			// not loaded yet so don't show anything
			ImGui::Text("Parted Model Manager is not active, cannot switch models!");
			return;
		}

		ImGui::Text("Find a Preset and press 'SWITCH' to swap Player Models.");

		if (ImGui::Button("Load Mobface"))
		{
			EARS::Modules::MobfaceManager* MobfaceMgr = EARS::Modules::MobfaceManager::GetInstance();
			MobfaceMgr->BuildModelFromSavedData(GetTreeSlot(), ActiveObject);
		}

		ModelMgr->ForEachAssemblyHeader([&](const PartedModel::AssemblyListHeader& InAssemblyHeader) {

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
							ActiveObject->TrySwapModel(ThisAssembly.m_AssemblyName, ThisPreset.m_PresetName);
						}
						ImGui::SameLine();
						ImGui::Text(ThisPreset.m_PresetName);
						ImGui::PopID();
					}

					ImGui::TreePop();
				}
			}
			});

		ImGui::EndTabItem();
	}
}

void ImGuiNPCInspector::DrawTab_Upgrades()
{
	if (ImGui::BeginTabItem("Upgrades", nullptr, ImGuiTabItemFlags_None))
	{
		ImGui::TextWrapped("Adjust upgrades assigned to entity (Such as Bulletproof Vest, Ammo Belt)");

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
		// NB: Upgrade Component is stored on SimNPC instead
		if (EARS::Modules::NPC* AsNPC = EARS::Framework::_QueryInterface<EARS::Modules::NPC>(ActiveObject, 0xA0D329D6))
		{
			return AsNPC->GetOwningSimNPC()->GetUpgradeComponent();
		}

		return nullptr;
	}
}

EARS::Modules::PlayerFamilyTree::FamilyTreeSlot ImGuiNPCInspector::GetTreeSlot() const
{
	if (bIsPlayer)
	{
		return EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_PLAYER;
	}
	else
	{
		if (EARS::Modules::NPC* AsNPC = EARS::Framework::_QueryInterface<EARS::Modules::NPC>(ActiveObject, 0xA0D329D6))
		{ 
			EARS::Modules::PlayerFamilyTree* FamilyTree = EARS::Modules::PlayerFamilyTree::GetInstance();
			return FamilyTree->FindTreeSlotIndex(AsNPC->GetOwningSimNPC());
		}

		return EARS::Modules::PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_SLOT_INVALID;
	}
}
