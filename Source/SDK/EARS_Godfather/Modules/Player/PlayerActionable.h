#pragma once

// SDK
#include "SDK/EARS_Framework/Modules/BehaviorInfo/BehaviorInfo.h"

namespace EARS
{
	namespace Modules
	{
		class PlayerActionableInfo : public EARS::Framework::BehaviorInfo
		{
		public:

		private:

			char m_Padding[0x138];
		};

		static_assert(sizeof(EARS::Modules::PlayerActionableInfo) == 0x144, "EARS::Modules::PlayerActionableInfo must equal 0x144");
	}
}
