#pragma once

// SDK
#include "SDK/ears_common/include/ears_common/Array.h"
#include "SDK/ears_common/include/ears_common/Singleton.h"
#include "SDK/ears_framework/src/game_framework/framework/core/attributehandler/CAttributeHandler.h"

namespace EARS
{
	namespace Modules
	{
		class UpgradeData : public RWS::CAttributeHandler
		{
		public:

		};

		class UpgradeManager : public Singleton<UpgradeManager>
		{
		public:

			Array<EARS::Modules::UpgradeData*> m_UpgradeList[6];
		};
	}
}
