#pragma once

// SDK
#include "ears_common/array.h"
#include "ears_common/singleton.h"
#include "framework/core/attributehandler/cattributehandler.h"

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
