#pragma once

// SDK
#include "SDK/ears_common/include/ears_common/Singleton.h"
#include "SDK/ears_framework/src/game_framework/framework/core/resourcemanager/CResourceHandler.h"
#include "SDK/ears_framework/src/framework/modules/controllermanager/ControllerEventConfig.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * RWS resource handler for the "CEC" type - the controller event configs.
		 *
		 * There is almost nothing to it, and that is the useful part. The resource image is
		 * already a valid ControllerEventConfig followed by its descriptor array, so loading
		 * amounts to: stamp the version, clear the refcount, point m_pEventDesc at the tail
		 * of the same allocation, and register the result with ControllerManager. Nothing
		 * here is out of reach of a mod that builds its own config in memory - see
		 * ControllerEventConfig::InitAsResourceImage and ControllerManager::AddConfigResource.
		 *
		 * The handler registers itself with an unload delay of 1 on PC (the X360 build uses 3).
		 */
		class ControllerDataManager : RWS::CResourceHandler, public Singleton<ControllerDataManager>
		{
		public:

			/** The RWS resource type name this handler claims. */
			static constexpr const char* kResourceTypeName = "CEC";

			/* Bind the raw blob as a ControllerEventConfig and register it.
			 * Returns the config. (game: 0x00463E30) */
			ControllerEventConfig* LoadResource(RWS::CResourceHandler::CResourceLoadInfo& LoadInfo);

			/* Unregister the config carried by the unload info. (game: 0x00463E70) */
			void UnloadResource(RWS::CResourceHandler::CResourceUnloadInfo& UnloadInfo);

			/** Singleton<EARS::Modules::ControllerDataManager>::s_pSingleton (0x01223460) */
			static ControllerDataManager* GetInstance();

		private:

			// RWS::CResourceHandler occupies 0x00..0x07, Singleton<> the vptr at 0x08.
			void* m_pAllocator = nullptr;	// 0x0C - EA::Allocator::IAllocator*
		};

		static_assert(sizeof(EARS::Modules::ControllerDataManager) == 0x10, "EARS::Modules::ControllerDataManager must equal 0x10");
	} // Modules
} // EARS
