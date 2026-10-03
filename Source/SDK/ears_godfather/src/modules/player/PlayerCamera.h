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

			enum Behaviour : uint32_t
			{
				PLAYER_CAMERA_BEHAVIOR_CHASE = 0x1,
				PLAYER_CAMERA_BEHAVIOR_TARGETING = 0x2,
				PLAYER_CAMERA_BEHAVIOR_FREEAIM_PROTO = 0x4,
				PLAYER_CAMERA_BEHAVIOR_FREEAIM = 0x8,
				PLAYER_CAMERA_BEHAVIOR_FREEAIM_ZOOM = 0x10,
				PLAYER_CAMERA_BEHAVIOR_FREEAIM_WALKING = 0x20,
				PLAYER_CAMERA_BEHAVIOR_STANDARD_CHASE = 0x40,
				PLAYER_CAMERA_BEHAVIOR_ACTIONABLE = 0x80,
				PLAYER_CAMERA_BEHAVIOR_SNIPER = 0x100,
				PLAYER_CAMERA_BEHAVIOR_ALL = 0x1FF,
			};

			// Test multiple flags
			bool AreBehaviorsEnabled(uint32_t PlayerCameraBehaviors) const { return PlayerCameraBehaviors == (this->m_EnabledCameraBehaviors & PlayerCameraBehaviors); }

			// API for current behaviours
			void SetCurrentCameraBehavior(uint32_t Behaviour) { m_CurrentCameraBehavior = Behaviour; }
			uint32_t GetCurrentCameraBehavior() const { return m_CurrentCameraBehavior; }

			// API for desired behaviours
			void SetDesiredCameraBehavior(uint32_t Behaviour) { m_DesiredCameraBehavior = Behaviour; }
			uint32_t GetDesiredCameraBehavior() const { return m_DesiredCameraBehavior; }

		private:

			uint32_t m_EnabledCameraBehaviors = 0;
			uint32_t m_CurrentCameraBehavior = 0;
			uint32_t m_DesiredCameraBehavior = 0;
		};
	}
}
