#pragma once

// TODO: Should be private/excluded from public API
#include "SDK/EARS_Trinity/src/TrinityTransformBase.h"

namespace EA::Trinity
{
	class TrinityCamera : public EA::Trinity::TrinityTransformBase
	{
	public:

		virtual ~TrinityCamera() = 0;

		virtual void InitCamera() = 0;
		virtual void ReInitCamera() = 0;
		virtual void CalculateFOV() = 0;
		virtual void BeginUpdate() = 0;
		virtual void EndUpdate() = 0;
		virtual void StopCamera() = 0;

	protected:

		// Accessors for the authored camera transform (read by derived cameras during update)
		const float* GetCamPosition() const { return m_CamPos; }
		const float* GetCamRotation() const { return m_CamRot; }
		float GetCamFieldOfView() const { return m_CamFOV; }

	private:

		EA::Trinity::TrinityCamera* m_Next = nullptr;
		int32_t m_CameraId = 0;
		float m_LerpOutTime = 0.0f;
		float m_FocalLength = 0;
		EA::Trinity::Interpolator m_FocalLengthInterp;
		float m_CamPos[3];
		float m_CamRot[3];
		float m_CamFOV = 0.0f;
		float m_CamFocalLength = 0.0f;
		float m_CamHFA = 0.0f;
	};

	static_assert(sizeof(TrinityCamera) == 0xB0);
}
