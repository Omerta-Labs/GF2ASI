#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Camera/Camera.h"

namespace EARS::Modules
{
	class CustomCameraInfo : public EARS::Framework::CameraInfo
	{
	public:

		//CustomCameraInfo();

		void SetPointers(float (*Pos)[4], float (*Rot)[4], float* FOV);
		void SetAuthoredAspectRatio(EARS::Framework::CameraAuthoredAspectRatio InType) { m_AuthoredAspectRatio = InType; }
		void SetAspectConversionType(EARS::Framework::CameraAspectConversionType InType) { m_AspectConversionType = InType; }

	private:

		float (*m_Pos)[4] = nullptr;					// 0x58
		float (*m_Rot)[4] = nullptr;					// 0x5C
		float* m_FOV = nullptr;							// 0x60
		EARS::Framework::CameraAuthoredAspectRatio m_AuthoredAspectRatio;	// 0x64
		EARS::Framework::CameraAspectConversionType m_AspectConversionType;	// 0x68
	};

	static_assert(sizeof(CustomCameraInfo) == 0x6C, "EARS::Modules::CustomCameraInfo must equal 0x6C");
}