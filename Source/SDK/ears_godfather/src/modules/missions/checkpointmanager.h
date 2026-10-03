#pragma once

// SDK (Common)
#include "SDK/ears_common/include/ears_common/array.h"
#include "SDK/ears_common/include/ears_common/bitflags.h"
#include "SDK/ears_common/include/ears_common/singleton.h"
#include "SDK/ears_common/include/ears_common/string.h"

// SDK (Framework)
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/ceventhandler.h"
#include "SDK/ears_framework/src/framework/core/persistence/PersistenceRegistry.h"

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


