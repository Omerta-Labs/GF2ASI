#pragma once

// SDK (Common)
#include "SDK/ears_common/include/ears_common/Singleton.h"

// SDK (Framework)
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/CEventHandler.h"
#include "SDK/ears_framework/src/game_framework/framework/core/resourcemanager/CResourceHandler.h"

namespace EARS
{
	namespace Framework
	{
		namespace Lua
		{
			class LuaScriptManager : public RWS::CResourceHandler, public RWS::CEventHandler, public Singleton<LuaScriptManager>
			{
			public:

				LuaScriptManager* GetInstance();

			private:

				void* m_LevelState = nullptr;
				void* m_GlobalState = nullptr;
			};
		} // Lua
	} // Framework
} // EARS
