#include "CameraManager.h"

#include "Addons/Hook.h"

namespace EARS
{
	namespace Framework
	{
		void CameraManager::EnableUpdate()
		{
			MemUtils::CallClassMethod<void, CameraManager*>(0x04252D0, this);
		}

		void CameraManager::DisableUpdate()
		{
			MemUtils::CallClassMethod<void, CameraManager*>(0x04252C0, this);
		}

		CameraManager* CameraManager::GetInstance()
		{
			return *(CameraManager**)0x122339C;
		}
	} // Framework
} // EARS
