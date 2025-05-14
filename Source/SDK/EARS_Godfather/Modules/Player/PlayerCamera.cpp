#include "PlayerCamera.h"

namespace EARS
{
	namespace Modules
	{
		PlayerCameraInfo::PlayerCameraInfo()
			: m_EnabledCameraBehaviors(511)
			, m_CurrentCameraBehavior(1)
			, m_DesiredCameraBehavior(1)
		{
			// NB: Taken from game exe
		}
	}
}
