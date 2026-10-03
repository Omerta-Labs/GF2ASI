#pragma once

// SDK (Common)
#include "ears_common/singleton.h"

// SDK (Framework)
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/resourcemanager/cresourcehandler.h"

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
