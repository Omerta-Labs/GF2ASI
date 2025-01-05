#pragma once

// SDK (Common)
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Common/RwMaths.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/Base/Base.h"

namespace EARS
{
	namespace Modules
	{
		class Checkpoint : public EARS::Framework::Base
		{
		public:

			const String& GetDebugName() const { return m_DebugName; }
			const String& GetChapterDebugName() const { return m_ChapterDebugName; }
			uint32_t GetCheckpointNumber() const { return m_CheckpointNumber; }
			uint32_t GetChapterNumber() const { return m_ChapterNumber; }

		private:

			String m_DebugName;
			String m_ChapterDebugName;
			uint32_t m_ChapterNumber = 0;
			uint32_t m_CheckpointNumber = 0;
			RwMatrixTag m_SpawnPoint;

			// TODO: Figure out remaining data

			//String m_sDebugName;
			//String m_sChapterDebugName;
			//unsigned int m_uChapterNumber;
			//unsigned int m_uCheckpointNumber;
			//RwMatrixTag m_matSpawn;
			//unsigned int m_uPreviousCheckpointNumber;
			//unsigned int m_uSaveCheckpointNumber;
			//bool m_bIsLoading;
			//EARS::Common::guid128_t m_guidStreamSet;
			//EARS::Common::guid128_t m_guidStartTeleport;
			//EARS::Common::guid128_t m_guidRestartTeleport;
			//EARS::Common::guid128_t m_guidDebugRestartTeleport;
			//RWS::CEventId m_msgStartNow;
			//RWS::CEventId m_msgFinishedLoading;
			//RWS::CEventId m_msgWasStarted;
			//RWS::CEventId m_msgWasRestarted;
		};
	}
}
