#include "CustomCamera.h"

#include "Addons/Hook.h"

namespace EARS::Modules
{
	CustomCameraInfo::CustomCameraInfo()
		: CameraInfo()
		, m_Pos(nullptr)
		, m_Rot(nullptr)
		, m_FOV(nullptr)
		, m_AuthoredAspectRatio(EARS::Framework::CameraAuthoredAspectRatio::CAMERA_AUTHORED_16x9)	// engine default (1)
		, m_AspectConversionType(EARS::Framework::CameraAspectConversionType::CAMERA_DISPLAY_CONST_Y)	// engine default (2)
	{
	}

	CustomCameraInfo::~CustomCameraInfo()
	{
	}

	EARS::Framework::Camera* CustomCameraInfo::Create()
	{
		// EARS::Modules::CustomCameraInfo::Create
		return MemUtils::CallClassMethod<EARS::Framework::Camera*, CustomCameraInfo*>(0x004B1660, this);
	}

	void CustomCameraInfo::SetPointers(float(*Pos)[4], float(*Rot)[4], float* FOV)
	{
		m_Pos = Pos;
		m_Rot = Rot;
		m_FOV = FOV;
	}
}
