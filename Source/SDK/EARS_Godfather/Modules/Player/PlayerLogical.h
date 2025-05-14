#pragma once

// SDK
#include "SDK/EARS_Framework/Modules/BehaviorInfo/BehaviorInfo.h"

namespace EARS
{
	namespace Modules
	{
		class PlayerLogicalInfo : public EARS::Framework::BehaviorInfo
		{
		public:

			// TODO: Create constructor

		private:

			uint32_t m_EngageTime = 0;
			uint32_t m_FreeAimTimeoutEndTime = 0;
			float m_ExcludeTime[8];
			float m_ExcludeTimer[8];
			float m_PressureTime[7];
			float m_LastDestructiblePressureTime = 0.0f;
		};
	}
}
