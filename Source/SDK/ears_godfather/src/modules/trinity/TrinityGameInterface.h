#pragma once

// SDK (Common)
#include "SDK/ears_common/include/ears_common/Singleton.h"

// SDK (Framework)
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/CEventHandler.h"
#include "SDK/ears_framework/src/game_framework/framework/core/resourcemanager/CResourceHandler.h"

namespace EARS::Modules
{
	class TrinityGameInterface : public RWS::CEventHandler, public RWS::CResourceHandler, public Singleton<EARS::Modules::TrinityGameInterface>
	{
	public:

		static TrinityGameInterface* GetInstance();

	private:
	};
}
