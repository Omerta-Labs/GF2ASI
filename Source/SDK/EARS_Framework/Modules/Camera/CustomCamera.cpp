#include "CustomCamera.h"

namespace EARS::Modules
{
	//CustomCameraInfo::CustomCameraInfo()
	//	: CameraInfo()
	//	, m_Pos(nullptr)
	//	, m_Rot(nullptr)
	//	, m_FOV(nullptr)
	//	, m_AuthoredAspectRatio(EARS::Framework::CameraAuthoredAspectRatio::CAMERA_AUTHORED_16x9)
	//	, m_AspectConversionType(EARS::Framework::CameraAspectConversionType::CAMERA_DISPLAY_CONST_Y)
	//{
	//}

	void CustomCameraInfo::SetPointers(float(*Pos)[4], float(*Rot)[4], float* FOV)
	{
		m_Pos = Pos;
		m_Rot = Rot;
		m_FOV = FOV;
	}
}
