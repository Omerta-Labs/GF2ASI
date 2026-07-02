#pragma once

// SDK
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Core/Camera/Camera.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Framework
	{
		// forward declares
		class CameraInfo;

		/**
		 * Blends Cameras together using stacks, primarily from CameraInfo(s).
		 */
		class __declspec(align(16)) CameraBlender
		{
		public:

			void PushCamera(uint32_t PlayerID, CameraInfo& CameraInfo, float BlendTime, CameraInterpType InterpType, bool bResetCamera);

			float GetActiveCamFov() const { return m_Data.m_FieldOfView; }

			const float* GetActiveCamRot() const { return m_Data.m_Rotation; }

			const float* GetActiveCamPos() const { return m_Data.m_Position; }

		private:

			struct CameraBlend
			{
				EARS::Framework::Camera* m_Camera = nullptr;
				float m_Weight = 0.0f;
				float m_WeightToBlend = 0.0f;
			};

			struct __declspec(align(4)) PostBlendModifier
			{
				void* m_Modifier = nullptr; // CameraPostBlendModifier
				bool m_bDeleteOnPop = false;
			};

			struct __declspec(align(4)) DelayedCameraActionInfo
			{
				enum DelayedCameraAction
				{
					INVALID = 0,
					PUSH_CAMERAINFO = 1,
					POP_CAMERAINFO = 2,
					POP_CAMERAPOSTBLENDMODIFIERINFO = 3
				};

				DelayedCameraAction m_Action = DelayedCameraAction::INVALID;
				SafePtr<EARS::Framework::CameraInfo> m_CameraInfo;
				float m_BlendTime = 0.0f;
				uint32_t m_PlayerID = 0;
				CameraInterpType m_InterpType = CameraInterpType::CAMERA_INTERP_NONE;
				bool m_bResetNextCamera = false;
			};

			CameraData m_Data;
			float m_YawVelocity = 0.0f;
			SafePtr<EARS::Framework::CameraInfo> m_CameraInfoStack[8];
			int32_t m_CameraInfoStackIdx = 0;
			CameraBlender::CameraBlend m_ActiveCameraList[12];
			uint32_t m_ActiveCameraIdx = 0;
			float m_BlendTime = 0.0f;
			float m_BlendValue = 0.0f;
			float m_BlendTimeTotal = 0.0f;
			CameraInterpType m_BlendInterpType = CameraInterpType::CAMERA_INTERP_NONE;
			CameraBlender::PostBlendModifier m_PostBlendModifiers[4];
			CameraBlender::DelayedCameraActionInfo m_DelayedCameraActions[4];
			int32_t m_DelayedCameraIdx = 0;
			bool m_bUpdating = false;
		};

		static_assert(sizeof(CameraBlender) == 0x1D0, "Size Mismatch");
	} // Framework
} // EARS
