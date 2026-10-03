#pragma once

// SDK (Common)
#include "ears_common/singleton.h"

// SDK (Framework)
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/resourcemanager/cresourcehandler.h"

namespace EARS::Modules
{
	class TrinityGameInterface : public RWS::CEventHandler, public RWS::CResourceHandler, public Singleton<EARS::Modules::TrinityGameInterface>
	{
	public:

		static TrinityGameInterface* GetInstance();

	private:
	};
}
