#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * A small struct on the Player to handle the communication
		 * between the Player entity and their State Machines.
		 */
		struct PlayerCameraInfo
		{
		public:

			PlayerCameraInfo();

		private:

			uint32_t m_EnabledCameraBehaviors = 0;
			uint32_t m_CurrentCameraBehavior = 0;
			uint32_t m_DesiredCameraBehavior = 0;
		};
	}
}
