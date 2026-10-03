#include "controllerdatamanager.h"

#include "Addons/Hook.h"

namespace EARS
{
	namespace Modules
	{
		ControllerEventConfig* ControllerDataManager::LoadResource(RWS::CResourceHandler::CResourceLoadInfo& LoadInfo)
		{
			return MemUtils::CallClassMethod<ControllerEventConfig*, ControllerDataManager*, RWS::CResourceHandler::CResourceLoadInfo*>(0x463E30, this, &LoadInfo);
		}

		void ControllerDataManager::UnloadResource(RWS::CResourceHandler::CResourceUnloadInfo& UnloadInfo)
		{
			MemUtils::CallClassMethod<void, ControllerDataManager*, RWS::CResourceHandler::CResourceUnloadInfo*>(0x463E70, this, &UnloadInfo);
		}

		ControllerDataManager* ControllerDataManager::GetInstance()
		{
			return *reinterpret_cast<ControllerDataManager**>(0x1223460);
		}
	} // Modules
} // EARS
