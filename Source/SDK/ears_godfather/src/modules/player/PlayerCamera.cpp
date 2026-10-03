#include "PlayerCamera.h"

namespace EARS
{
	namespace Modules
	{
		PlayerCameraInfo::PlayerCameraInfo()
			: m_EnabledCameraBehaviors(Behaviour::PLAYER_CAMERA_BEHAVIOR_ALL)
			, m_CurrentCameraBehavior(Behaviour::PLAYER_CAMERA_BEHAVIOR_CHASE)
			, m_DesiredCameraBehavior(Behaviour::PLAYER_CAMERA_BEHAVIOR_CHASE)
		{
			// NB: Taken from game exe
		}
	}
}
