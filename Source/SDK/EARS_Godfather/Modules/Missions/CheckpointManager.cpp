#include "CheckpointManager.h"

// addons
#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Missions/Checkpoint.h"

namespace EARS::Modules
{
	void CheckpointManager::AddCheckpoint(EARS::Modules::Checkpoint& NewCheckpoint)
	{
		if (m_Checkpoints.Find(&NewCheckpoint) == -1)
		{
			m_Checkpoints.Add(&NewCheckpoint);
		}
	}

	void CheckpointManager::RemoveCheckpoint(EARS::Modules::Checkpoint& NewCheckpoint)
	{
		const int32_t CheckpointIdx = m_Checkpoints.Find(&NewCheckpoint);
		if (CheckpointIdx >= 0)
		{
			m_Checkpoints.DeleteFast(CheckpointIdx);
			if (m_ActiveCheckpoint == &NewCheckpoint)
			{
				m_ActiveCheckpoint->StopLoading();
				m_ActiveCheckpoint = nullptr;
			}
		}
	}

	void CheckpointManager::RestartNewCheckpoint(EARS::Modules::Checkpoint* NewCheckpoint, RestartType InType, uint32_t ExtraTeleportOptions)
	{
		MemUtils::CallClassMethod<void, EARS::Modules::CheckpointManager*, EARS::Modules::Checkpoint*, RestartType, uint32_t>(0x08FD380, this, NewCheckpoint, InType, ExtraTeleportOptions);
	}

	CheckpointManager* CheckpointManager::GetInstance()
	{
		return *(EARS::Modules::CheckpointManager**)0x11299C4;
	}
}
