#pragma once

// SDK (Common)
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Common/String.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/Persistence/PersistenceRegistry.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class Checkpoint;

		class CheckpointManager : public EARS::Framework::IPersistable, Singleton<CheckpointManager>, RWS::CEventHandler
		{
		public:

			enum class RestartType : uint32_t
			{
				RESTART_NO_TELEPORT = 0x0,
				RESTART_NORMAL_TELEPORT = 0x1,
				RESTART_DEBUG_TELEPORT = 0x2,
			};

			void AddCheckpoint(EARS::Modules::Checkpoint& NewCheckpoint);
			void RemoveCheckpoint(EARS::Modules::Checkpoint& NewCheckpoint);

			void RestartNewCheckpoint(EARS::Modules::Checkpoint* NewCheckpoint, RestartType InType, uint32_t ExtraTeleportOptions);

			// getters
			EARS::Modules::Checkpoint* GetCurrentCheckpoint() const { return m_ActiveCheckpoint; }

			static CheckpointManager* GetInstance();

		private:

			Flags32 m_Flags;
			uint32_t m_SpawnType; // is actually PlayerSpawnType
			EARS::Modules::Checkpoint* m_ActiveCheckpoint = nullptr;
			Array<EARS::Modules::Checkpoint*> m_Checkpoints;
			uint32_t m_FailEffect = 0; // is actually VFXHandle
			String m_FailReason;
			uint32_t m_FailEffectTimerMS = 0;
		};
	}
}


