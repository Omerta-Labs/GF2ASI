#include "MarketingDebug.h"

// SDK
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Core/Input/Input.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

// C++
#include <algorithm>
#include <cmath>

namespace
{
	// Below this, stick input is treated as noise and clamped to zero. Matches the
	// deadzone used by the original UpdateFreeCamera (and PlayerDebugFlySM::ProcessMovement).
	constexpr float FREECAM_STICK_DEADZONE = 0.25f;

	constexpr float FREECAM_MOVE_SPEED = 8.0f;         // world units/sec, full stick deflection
	constexpr float FREECAM_MOVE_SPEED_MODIFIER = 3.0f; // multiplier while the speed-up button is held

	constexpr float FREECAM_ROTATE_SPEED = 1.5f;       // radians/sec, full stick deflection

	// Keep the camera from flipping past vertical when pitching with the stick.
	constexpr float FREECAM_PITCH_LIMIT = 1.5f; // ~85 degrees

	// Rescales |StickValue| from [Deadzone, 1] to [0, 1] and re-applies the sign, so
	// movement/rotation ramps up smoothly from zero right past the deadzone edge.
	float ApplyStickDeadzone(float StickValue, float Deadzone)
	{
		const float AbsValue = std::fabs(StickValue);
		if (AbsValue <= Deadzone)
		{
			return 0.0f;
		}

		const float Sign = (StickValue < 0.0f) ? -1.0f : 1.0f;
		return Sign * ((AbsValue - Deadzone) / (1.0f - Deadzone));
	}
}

namespace EARS::Modules
{
	MarketingCameraInfo::MarketingCameraInfo()
		: CameraInfo()
	{
		SetIsNonInterruptable(true);
	}

	EARS::Framework::Camera* MarketingCameraInfo::Create()
	{
		MarketingCamera* NewCamera = new MarketingCamera(*this);
		return NewCamera;
	}

	MarketingCamera::MarketingCamera(const EARS::Framework::CameraInfo& InCameraInfo)
		: Camera(InCameraInfo)
	{
	}

	void MarketingCamera::Init(uint32_t PlayerID, bool bResetCamera)
	{
		EARS::Framework::Camera::Init(PlayerID, bResetCamera);

		EARS::Framework::CameraManager* CamMgr = EARS::Framework::CameraManager::GetInstance();

		RwV3d CurrentPos;
		CamMgr->GetActiveCamPos(CurrentPos, PlayerID);

		m_CameraData.m_Position[0] = CurrentPos.m_X;
		m_CameraData.m_Position[1] = CurrentPos.m_Y;
		m_CameraData.m_Position[2] = CurrentPos.m_Z;

		RwV3d CurrentRot;
		CamMgr->GetActiveCamRot(CurrentRot, PlayerID);

		m_CameraData.m_Rotation[0] = CurrentRot.m_X;
		m_CameraData.m_Rotation[1] = CurrentRot.m_Y;
		m_CameraData.m_Rotation[2] = CurrentRot.m_Z;
		m_CameraData.m_Rotation[3] = 0.0f;
	}

	void MarketingCamera::Update(float DeltaTime)
	{
		using namespace EARS::Framework;
		Camera::Update(DeltaTime);

		const EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer();
		const EARS::Framework::InputDeviceManager* InputMgr = EARS::Framework::InputDeviceManager::GetInstance();
		
		const uint16_t ControllerID = LclPlayer->GetControllerID();
		const Controller_Info* ControllerRuntimeData = InputMgr->GetControllerInfo(ControllerID);
		if (ControllerRuntimeData != nullptr)
		{
			// Hold Left-Bottom (L1/LB) to move faster.
			const bool bSpeedModifierHeld = InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_LEFT_BOTTOM, ButtonStatus::BUTTON_STATUS_DOWN);
			const float SpeedModifier = bSpeedModifierHeld ? FREECAM_MOVE_SPEED_MODIFIER : 1.0f;

			float& Pitch = m_CameraData.m_Rotation[0];
			float& Yaw = m_CameraData.m_Rotation[1];

			// Right stick X -> yaw rotation.
			const float YawInput = ApplyStickDeadzone(ControllerRuntimeData->m_RightStick.m_X, FREECAM_STICK_DEADZONE);
			if (YawInput != 0.0f)
			{
				Yaw -= YawInput * FREECAM_ROTATE_SPEED * DeltaTime;
			}

			// Right stick Y -> pitch rotation, clamped so the camera can't flip past vertical.
			const float PitchInput = ApplyStickDeadzone(ControllerRuntimeData->m_RightStick.m_Y, FREECAM_STICK_DEADZONE);
			if (PitchInput != 0.0f)
			{
				Pitch = std::clamp(Pitch + (PitchInput * FREECAM_ROTATE_SPEED * DeltaTime), -FREECAM_PITCH_LIMIT, FREECAM_PITCH_LIMIT);
			}

			// Left stick Y -> forwards/backwards along the look direction (yaw + pitch),
			// left stick X -> strafe along the flat right vector (yaw only).
			const float MoveInput = ApplyStickDeadzone(ControllerRuntimeData->m_LeftStick.m_Y, FREECAM_STICK_DEADZONE);
			const float StrafeInput = ApplyStickDeadzone(ControllerRuntimeData->m_LeftStick.m_X, FREECAM_STICK_DEADZONE);
			if (MoveInput != 0.0f || StrafeInput != 0.0f)
			{
				// NB: Uses XMScalarSinCos in XBOX
				const float SinYaw = std::sin(Yaw);
				const float CosYaw = std::cos(Yaw);
				const float SinPitch = std::sin(Pitch);
				const float CosPitch = std::cos(Pitch);

				const float MoveAmount = MoveInput * FREECAM_MOVE_SPEED * SpeedModifier * DeltaTime;
				const float StrafeAmount = StrafeInput * FREECAM_MOVE_SPEED * SpeedModifier * DeltaTime;

				m_CameraData.m_Position[0] -= (SinYaw * CosPitch * MoveAmount) + (CosYaw * StrafeAmount);
				m_CameraData.m_Position[1] += SinPitch * MoveAmount;
				m_CameraData.m_Position[2] += (SinYaw * StrafeAmount) - (CosYaw * CosPitch * MoveAmount);
			}
		}
	}
}
