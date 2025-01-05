#include "CheckpointManager.h"

// addons
#include "Addons/Hook.h"

void EARS::Modules::CheckpointManager::RestartNewCheckpoint(EARS::Modules::Checkpoint* NewCheckpoint, RestartType InType, uint32_t ExtraTeleportOptions)
{
	MemUtils::CallClassMethod<void, EARS::Modules::CheckpointManager*, EARS::Modules::Checkpoint*, RestartType, uint32_t>(0x08FD380, this, NewCheckpoint, InType, ExtraTeleportOptions);
}

void EARS::Modules::CheckpointManager::ForEachCheckpoint(const TVisitCheckpointFunctor& InFunction)
{
	for (EARS::Modules::Checkpoint* CurrentCheckpoint : m_Checkpoints)
	{
		InFunction(*CurrentCheckpoint);
	}
}

EARS::Modules::CheckpointManager* EARS::Modules::CheckpointManager::GetInstance()
{
	return *(EARS::Modules::CheckpointManager**)0x11299C4;
}
