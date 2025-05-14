#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Framework
	{
		enum CameraAuthoredAspectRatio : uint32_t
		{
			CAMERA_AUTHORED_16x9 = 0x1,
			CAMERA_AUTHORED_4x3 = 0x2,
		};

		enum CameraAspectConversionType : uint32_t
		{
			CAMERA_DISPLAY_CONST_X = 0x1,
			CAMERA_DISPLAY_CONST_Y = 0x2,
		};

		struct CameraData
		{
		public:

			float m_Position[4];
			float m_Anchor[4];
			float m_Rotation[4];
			float m_FieldOfView = 0.0f;
			EARS::Framework::CameraAuthoredAspectRatio m_AuthoredAspectRatio;
			EARS::Framework::CameraAspectConversionType m_AspectConversionType;
		};

		/**
		 * Blends Cameras together using stacks, primarily from CameraInfo(s).
		 */
		class CameraBlender
		{
		public:

			float GetActiveCamFov() const { return m_Data.m_FieldOfView; }

			const float* GetActiveCamRot() const { return m_Data.m_Rotation; }

		private:

			CameraData m_Data;
		};
	} // Framework
} // EARS
