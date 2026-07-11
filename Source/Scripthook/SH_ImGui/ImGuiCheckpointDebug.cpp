#include "ImGuiCheckpointDebug.h"

// Addons
#include "addons/imgui/imgui.h"

// Godfather
#include "SDK/EARS_Godfather/Modules/Missions/CheckpointManager.h"
#include "SDK/EARS_Godfather/Modules/Missions/Checkpoint.h"

// C++
#include <algorithm>

namespace SH
{
	void ImGuiCheckpointDebug::OpenLevelServices()
	{
		Chapters.clear();
	}
	void ImGuiCheckpointDebug::CloseLevelServices()
	{
		// we don't own any pointers, do don't delete them
		Chapters.clear();
	}

	void ImGuiCheckpointDebug::AddCheckpoint(EARS::Modules::Checkpoint& InCheckpoint)
	{
		const uint32_t ChapterID = InCheckpoint.GetChapterNumber();

		bool bHasChapter = false;
		for (ImGuiChapter& ChapterSection : Chapters)
		{
			if (ChapterSection.m_ChapterID == ChapterID)
			{
				ChapterSection.m_Checkpoints.insert(&InCheckpoint);
				bHasChapter = true;

				// check if we can replace empty debug name
				if (ChapterSection.m_ChapterDebugName == nullptr && InCheckpoint.GetChapterDebugName().IsValid())
				{
					// better than nothing
					ChapterSection.m_ChapterDebugName = InCheckpoint.GetChapterDebugName().raw_c_str();
				}

				break;
			}
		}

		if (bHasChapter == false)
		{
			ImGuiChapter& NewChapter = Chapters.emplace_back();
			NewChapter.m_ChapterID = InCheckpoint.GetChapterNumber();
			NewChapter.m_ChapterDebugName = InCheckpoint.GetChapterDebugName().raw_c_str();
			NewChapter.m_Checkpoints.insert(&InCheckpoint);

			// re-order
			std::sort(Chapters.begin(), Chapters.end(), [](const ImGuiChapter& ChapterA, const ImGuiChapter& ChapterB)
				{
					return ChapterA.m_ChapterID < ChapterB.m_ChapterID;
				});
		}
	}

	void ImGuiCheckpointDebug::RemoveCheckpoint(EARS::Modules::Checkpoint& InCheckpoint)
	{
		const uint32_t ChapterID = InCheckpoint.GetChapterNumber();
		for (ImGuiChapter& ChapterSection : Chapters)
		{
			if (ChapterSection.m_Checkpoints.contains(&InCheckpoint))
			{
				ChapterSection.m_Checkpoints.erase(&InCheckpoint);
				break;
			}
		}
	}

	void ImGuiCheckpointDebug::DisplayTab()
	{
		EARS::Modules::CheckpointManager& CheckpointMgr = *EARS::Modules::CheckpointManager::GetInstance();

		// get active and tell user
		if (EARS::Modules::Checkpoint* CurrentCheckpoint = CheckpointMgr.GetCurrentCheckpoint())
		{
			const String& DebugName = CurrentCheckpoint->GetDebugName();
			ImGui::Text("Current Checkpoint: [%u-%u] %s", CurrentCheckpoint->GetChapterNumber(), CurrentCheckpoint->GetCheckpointNumber(), DebugName.raw_c_str());
		}
		else
		{
			ImGui::Text("Current Checkpoint: (null)");
		}

		ImGui::Separator();

		ImGui::BeginChild("checkpoint_selection_child");
		for (const ImGuiChapter& CurrentChapter : Chapters)
		{
			if (ImGui::TreeNodeEx(&CurrentChapter, ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DrawLinesToNodes | ImGuiTreeNodeFlags_Framed, "[%u] %s", CurrentChapter.m_ChapterID, CurrentChapter.m_ChapterDebugName))
			{
				for (EARS::Modules::Checkpoint* CurrentCheckpoint : CurrentChapter.m_Checkpoints)
				{
					// compile name
					const String& DebugName = CurrentCheckpoint->GetDebugName();

					// present selectable element which user can press
					ImGuiTreeNodeFlags CheckpointFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DrawLinesToNodes;
					if (CheckpointMgr.GetCurrentCheckpoint() == CurrentCheckpoint)
					{
						CheckpointFlags |= ImGuiTreeNodeFlags_Selected;
					}

					if(ImGui::TreeNodeEx(CurrentCheckpoint, CheckpointFlags, DebugName.raw_c_str()))
					{
						if (ImGui::IsItemClicked())
						{
							CheckpointMgr.RestartNewCheckpoint(CurrentCheckpoint, EARS::Modules::CheckpointManager::RestartType::RESTART_DEBUG_TELEPORT, 0);
						}
						
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
	}
}