#include "ImGuiUISystem.h"

// Scripthook
#include "Scripthook/SH_ImGui/ImGuiPropertyGrid.h"

// SDK
#include "SDK/EARS_Framework/Toolkits/GroupManager/GroupManager.h"
#include "SDK/EARS_Godfather/Modules/NPC/NPC.h"
#include "SDK/EARS_Godfather/Modules/UI/UIHud.h"
#include "SDK/EARS_Locale/LocaleManager.h"

// CPP
#include <string>

namespace SH
{
	void ImGuiUISystem::DrawTab()
	{
		if (!ImGui::BeginTabItem("UI Hud", nullptr, ImGuiTabItemFlags_None))
		{
			return;
		}

		ImGui::SeparatorText("HUD");

		if (BeginPropertyTable("HUDProperties"))
		{
			EARS::Apt::UIHUD* UIHudManager = EARS::Apt::UIHUD::GetInstance();

			ImGui::BeginDisabled(UIHudManager == nullptr);

			BeginPropertyRow("Suppress HUD", "Hides the game's HUD elements (health, map, money) until unticked.");
			bool bNewSuppressed = bWantsUISuppressed;
			if (ImGui::Checkbox("##SuppressHUD", &bNewSuppressed))
			{
				if (bNewSuppressed)
				{
					SupressHUD();
				}
				else
				{
					UnsupressHUD();
				}
			}

			ImGui::EndDisabled();

			BeginPropertyRow("NPC HUD Indicators", "Shows or hides the indicator markers above NPCs' heads. "
				"Only applies to NPCs currently spawned - NPCs that spawn afterwards will show theirs again.");
			bool bNewIndicatorsVisible = !bNPCIndicatorsHidden;
			if (ImGui::Checkbox("##NPCIndicators", &bNewIndicatorsVisible))
			{
				if (bNewIndicatorsVisible)
				{
					ShowAllNPCIndicators();
				}
				else
				{
					HideAllNPCIndicators();
				}
			}

			ImGui::EndTable();
		}

		ImGui::SeparatorText("Localisation");

		if (EARS::Locale::LocaleManager* LocaleMgr = EARS::Locale::LocaleManager::GetInstance())
		{
			if (BeginPropertyTable("LocaleProperties"))
			{
				BeginPropertyRow("Text Language", "Switches the language used for all in-game text.");
				if (ImGui::BeginCombo("##TextLanguage", "Select Language"))
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

				BeginPropertyRow("Audio Language", "Switches the language used for speech, where audio is available.");
				if (ImGui::BeginCombo("##AudioLanguage", "Select Language"))
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

				ImGui::EndTable();
			}
		}
		else
		{
			ImGui::Text("Locale Manager is missing!");
		}

		ImGui::EndTabItem();
	}

	void ImGuiUISystem::ShowAllNPCIndicators()
	{
		using namespace EARS::Framework;

		GroupManager* GroupMgr = GroupManager::GetInstance();
		const GroupManager::TEntityList& MemberList = GroupMgr->FindGroupMembers(2);

		for (Entity* Member : MemberList)
		{
			if (EARS::Modules::NPC* AsNPC = EARS::Framework::_GetInterface<EARS::Modules::NPC>(Member, 0x369AC561))
			{
				const String& VFXName = AsNPC->GetHudIndicatorVFXName();
				AsNPC->ActivateHUDIndicator(AsNPC->GetHudIndicatorType(), VFXName.c_str());
			}
		}

		bNPCIndicatorsHidden = false;
	}

	void ImGuiUISystem::HideAllNPCIndicators()
	{
		using namespace EARS::Framework;

		GroupManager* GroupMgr = GroupManager::GetInstance();
		const GroupManager::TEntityList& MemberList = GroupMgr->FindGroupMembers(2);

		for (Entity* Member : MemberList)
		{
			if (EARS::Modules::NPC* AsNPC = EARS::Framework::_GetInterface<EARS::Modules::NPC>(Member, 0x369AC561))
			{
				AsNPC->DisableHUDIndicator();
			}
		}

		bNPCIndicatorsHidden = true;
	}

	void ImGuiUISystem::CloseLevelServices()
	{
		// The level teardown restores the HUD and despawns NPCs itself;
		// just drop our state so the next level starts from defaults
		bWantsUISuppressed = false;
		bNPCIndicatorsHidden = false;
	}

	void ImGuiUISystem::SupressHUD()
	{
		EARS::Apt::UIHUD::GetInstance()->Suppress();
		bWantsUISuppressed = true;
	}

	void ImGuiUISystem::UnsupressHUD()
	{
		EARS::Apt::UIHUD::GetInstance()->Unsuppress();
		bWantsUISuppressed = false;
	}
}
