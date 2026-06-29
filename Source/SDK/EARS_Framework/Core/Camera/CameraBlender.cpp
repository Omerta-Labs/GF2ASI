#include "CameraBlender.h"

namespace EARS::Framework
{
	void CameraBlender::PushCamera(uint32_t PlayerID, CameraInfo& CameraInfo, float BlendTime, CameraInterpType InterpType, bool bResetCamera)
	{
		if (m_bUpdating)
		{
			m_DelayedCameraIdx++;
			CameraBlender::DelayedCameraActionInfo& DelayedAction = m_DelayedCameraActions[m_DelayedCameraIdx];
			DelayedAction.m_Action = DelayedCameraActionInfo::DelayedCameraAction::PUSH_CAMERAINFO;
			DelayedAction.m_BlendTime = BlendTime;
			DelayedAction.m_PlayerID = PlayerID;
			DelayedAction.m_InterpType = InterpType;
			DelayedAction.m_CameraInfo = &CameraInfo;
		}
		else
		{
			if (m_CameraInfoStackIdx == 7)
			{

			}
		}
	}
}
