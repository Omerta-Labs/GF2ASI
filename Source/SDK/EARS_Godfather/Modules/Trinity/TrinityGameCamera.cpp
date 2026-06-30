#include "TrinityGameCamera.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Common/RwMaths.h"
#include "SDK/EARS_Framework/Core/Camera/Camera.h"
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Modules/Camera/CustomCamera.h"

// C++
#include <cmath>

namespace EARS::Modules
{
	// Shared in-game-camera state (one CustomCameraInfo drives the engine camera for all TrinityGameCameras)
	static hook::Type<bool>								sbHasPushedCameraInfo(0x112DCB0);
	static hook::Type<float>							sIGCFOV(0x112DC90);
	static hook::Type<float[4]>							sIGCPos(0x112DC80);
	static hook::Type<float[4]>							sIGCRot(0x112DCA0);
	static hook::Type<SafePtr<CustomCameraInfo>>		spCustomCameraInfo(0x112DCB4);

	void TrinityGameCamera::BeginUpdate()
	{
		// First time through, hook the shared CustomCameraInfo up to our static buffers and push it onto the stack
		if (!sbHasPushedCameraInfo)
		{
			sIGCFOV = 0.95001328f; // 0x3F733212

			if (CustomCameraInfo* const CameraInfoPtr = spCustomCameraInfo->GetPtr())
			{
				CameraInfoPtr->SetPointers(
					reinterpret_cast<float(*)[4]>(sIGCPos.ptr()),
					reinterpret_cast<float(*)[4]>(sIGCRot.ptr()),
					&sIGCFOV);
				CameraInfoPtr->SetAuthoredAspectRatio(EARS::Framework::CAMERA_AUTHORED_16x9);
				CameraInfoPtr->SetAspectConversionType(EARS::Framework::CAMERA_DISPLAY_CONST_Y);

				EARS::Framework::CameraManager* const CameraMgr = EARS::Framework::CameraManager::GetInstance();
				m_PlayerID = CameraMgr->GetBoundPlayer(0);
				CameraMgr->PushCameraInfo(m_PlayerID, CameraInfoPtr, 0.0f, EARS::Framework::CAMERA_INTERP_LINEAR, false);
			}

			sbHasPushedCameraInfo = true;
		}

		// Push the authored position straight through
		const float* const CamPos = GetCamPosition();
		sIGCPos[0] = CamPos[0];
		sIGCPos[1] = CamPos[1];
		sIGCPos[2] = CamPos[2];

		// Build a rotation matrix from the authored Euler angles
		const float* const CamRot = GetCamRotation();
		const float SinX = sinf(CamRot[0]);
		const float CosX = cosf(CamRot[0]);
		const float SinY = sinf(CamRot[1]);
		const float CosY = cosf(CamRot[1]);
		const float SinZ = sinf(CamRot[2]);
		const float CosZ = cosf(CamRot[2]);

		RwMatrixTag RotationMatrix;
		RotationMatrix.m_Right = RwV3d(CosY * CosZ, CosY * SinZ, -SinY);
		RotationMatrix.m_Up    = RwV3d((SinY * SinX * CosZ) - (CosX * SinZ), (SinY * SinX * SinZ) + (CosX * CosZ), CosY * SinX);
		RotationMatrix.m_At    = RwV3d((SinY * CosX * CosZ) + (SinX * SinZ), (SinY * CosX * SinZ) - (SinX * CosZ), CosY * CosX);

		// Flip the camera 180 degrees about the Y axis, then combine
		RwMatrixTag FlipMatrix;
		RwMatrixRotate(FlipMatrix, RwV3d(0.0f, 1.0f, 0.0f), 180.0f, rwCOMBINEREPLACE);

		RwMatrixTag CombinedMatrix;
		RwMatrixMultiply(CombinedMatrix, FlipMatrix, RotationMatrix);

		// Extract the final Euler angles back out for the engine camera
		RwV3d ResultEuler;
		EARS::RwMatrixExtractEulerAngles(ResultEuler, CombinedMatrix);
		EARS::ConvertRwV3dToVec4flt(sIGCRot.ptr(), ResultEuler);

		sIGCFOV = GetCamFieldOfView();
	}
}
