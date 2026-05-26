#pragma once

// C++
#include <set>
#include <vector>

// forward declares
namespace EARS::Modules
{
	class Checkpoint;
}

namespace SH
{
	struct ImGuiCheckpointDebug
	{
	public:

		void OpenLevelServices();

		void CloseLevelServices();

		void AddCheckpoint(EARS::Modules::Checkpoint& InCheckpoint);

		void RemoveCheckpoint(EARS::Modules::Checkpoint& InCheckpoint);

		void DisplayTab();

	private:

		struct ImGuiChapter
		{
			uint32_t m_ChapterID = 0;

			const char* m_ChapterDebugName = nullptr;

			std::set<EARS::Modules::Checkpoint*> m_Checkpoints;
		};

		std::vector<ImGuiChapter> Chapters;
	};
}
