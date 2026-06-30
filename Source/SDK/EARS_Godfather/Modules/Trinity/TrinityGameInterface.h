#pragma once

// SDK (Common)
#include "SDK/EARS_Common/Singleton.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/ResourceManager/CResourceHandler.h"

namespace EARS::Modules
{
	class TrinityGameInterface : public RWS::CEventHandler, public RWS::CResourceHandler, public Singleton<EARS::Modules::TrinityGameInterface>
	{
	public:

		static TrinityGameInterface* GetInstance();

	private:
	};
}
