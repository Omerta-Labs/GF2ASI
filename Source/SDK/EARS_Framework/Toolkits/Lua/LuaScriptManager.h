#pragma once

// SDK (Common)
#include "SDK/EARS_Common/Singleton.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/Core/ResourceManager/CResourceHandler.h"

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
