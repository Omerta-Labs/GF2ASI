#include "MarketingDebug.h"

#include "addons/hook.h"
#include "addons/tConsole.h"

// SDK
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Core/Input/Input.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"

// Scripthook
#include "Scripthook/SH_ImGui/ImGuiManager.h"

// C++
#include <algorithm>
#include <cmath>

namespace
{
	// Below this, stick input is treated as noise and clamped to zero. Matches the
	// deadzone used by the original UpdateFreeCamera (and PlayerDebugFlySM::ProcessMovement).
	constexpr float FREECAM_STICK_DEADZONE = 0.25f;

	// Radians per mouse count (the delta from InputDeviceManager already includes the user's
	// Mouse.Sensitivity option). Mouse input is a per-frame displacement, not a rate, so it is
	// applied directly each frame WITHOUT delta time - scaling it by delta time is what made
	// the mouse feel half as fast every time the frame rate doubled. 3.0e-4 rad (~0.017 deg)
	// per count matches how the old stick-based path felt at 30fps:
	// sensitivity * 0.004 (game's mouse->stick scale) * 2.25 rad/s / 30fps.
	constexpr float FREECAM_MOUSE_ROTATE_SPEED = 3.0e-4f;

	// A single frame of real mouse movement is at most a few hundred counts, even during a
	// hard flick. Anything larger is corrupted delta tracking (e.g. the game recentring the
	// cursor across the whole screen after the ImGui menu suppressed SetCursorPos), so it
	// gets discarded rather than applied.
	constexpr float FREECAM_MOUSE_DELTA_MAX = 1000.0f;

	// Keep the camera from flipping past vertical when pitching with the stick.
	constexpr float FREECAM_PITCH_LIMIT = 1.5f; // ~85 degrees

	// Active settings for current marketing camera
	EARS::Modules::MarketingCameraSettings ACTIVE_SETTINGS;

	// Frame-rate independent exponential smoothing of 'Current' towards 'Target'.
	// 'Tau' is the time constant in seconds (time to close ~63% of the gap).
	float SmoothTowards(float Current, float Target, float Tau, float DeltaTime)
	{
		if (Tau <= 0.0f)
		{
			return Target;
		}

		const float Alpha = 1.0f - std::exp(-DeltaTime / Tau);
		return Current + (Target - Current) * Alpha;
	}

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

	// Runtime mirror of the front-end "Mouse.Invert" option. Written by the options screen
	// and on settings load; the game's own cameras read it to flip mouse pitch.
	bool IsMouseInvertEnabled()
	{
		return *reinterpret_cast<bool*>(0x112A7A4);
	}

	class ControllerManager
	{
	public:

		void GetStickDir(uint8_t InControllerID, int InStickType, float* OutX, float* OutY, bool bRespectDeadZone, bool bRemapWithCtrlCurve, float Slope, float Exponent, float DeadZone) const
		{
			MemUtils::CallClassMethod<void>(0x0465E30, this, InControllerID, InStickType, OutX, OutY, bRespectDeadZone, bRemapWithCtrlCurve, Slope, Exponent, DeadZone);
		}

		static ControllerManager* GetInstance()
		{
			return *(ControllerManager**)0x1223464;
		}

	private:
	};
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
		m_ActiveCamera = NewCamera;
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

		// The inherited rotation can come back wrapped outside [-pi, pi] (e.g. a slight
		// downwards pitch stored as ~2pi). Normalise the pitch, otherwise the pitch clamp
		// yanks the camera to the +/-85 degree limit on the first rotation input.
		constexpr float TwoPi = 6.28318530f;
		m_CameraData.m_Rotation[0] = std::remainder(CurrentRot.m_X, TwoPi);
		m_CameraData.m_Rotation[1] = CurrentRot.m_Y;
		m_CameraData.m_Rotation[2] = CurrentRot.m_Z;
		m_CameraData.m_Rotation[3] = 0.0f;

		// Inherit the FOV alongside position/rotation, so this camera starts at the real
		// in-game value (and FOV controls like the Photo Mode slider have a valid value to
		// read) instead of the constructed default of 0.
		float CurrentFov = 0.0f;
		CamMgr->GetActiveCamFov(CurrentFov, PlayerID);
		m_CameraData.m_FieldOfView = CurrentFov;

		// Discard whatever mouse movement built up before the camera took over (menu
		// interaction, cursor recentring) instead of applying it as one giant first delta.
		m_bSwallowMouseDelta = true;
		m_SmoothedMouseVelocityX = 0.0f;
		m_SmoothedMouseVelocityY = 0.0f;
	}

	void MarketingCamera::Update(float DeltaTime)
	{
		using namespace EARS::Framework;
		Camera::Update(DeltaTime);

		const EARS::Modules::Player* LclPlayer = EARS::Modules::Player::GetLocalPlayer();
		const EARS::Framework::InputDeviceManager* InputMgr = EARS::Framework::InputDeviceManager::GetInstance();
		const ControllerManager* ControllerMgr = ControllerManager::GetInstance();

		const uint16_t ControllerID = LclPlayer->GetControllerID();

		// While the ImGui menu is open it owns the mouse and keyboard, so ignore all camera
		// input - otherwise interacting with the menu also flies the camera around.
		const ImGuiManager* ImGuiMgr = ImGuiManager::Get();
		const bool bCursorCaptured = (ImGuiMgr != nullptr) && ImGuiMgr->HasCursorControl();

		float LeftStickX = 0.0f;
		float LeftStickY = 0.0f;
		float RightStickX = 0.0f;
		float RightStickY = 0.0f;
		if (!bCursorCaptured)
		{
			ControllerMgr->GetStickDir(ControllerID, 0, &LeftStickX, &LeftStickY, false, false, -1.0f, -1.0f, -1.0f);
			ControllerMgr->GetStickDir(ControllerID, 1, &RightStickX, &RightStickY, false, false, -1.0f, -1.0f, -1.0f);
		}

		// Hold Left-Bottom (L1/LB) to move faster.
		const bool bSpeedModifierHeld = !bCursorCaptured && InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_LEFT_BOTTOM, ButtonStatus::BUTTON_STATUS_DOWN);
		const float SpeedModifier = bSpeedModifierHeld ? ACTIVE_SETTINGS.m_MoveSpeedModifier : 1.0f;

		float& Pitch = m_CameraData.m_Rotation[0];
		float& Yaw = m_CameraData.m_Rotation[1];

		// The mouse is merged into the same virtual right stick as the gamepad (per-axis, by
		// largest magnitude), but its contribution is a per-frame displacement rather than a
		// deflection rate, so it must not be scaled by delta time. Read the raw delta and apply
		// it directly; while the mouse is moving, feed zero into the stick path so the same
		// motion isn't applied twice.
		float MouseDeltaX = 0.0f;
		float MouseDeltaY = 0.0f;
		InputMgr->GetMouseDelta(MouseDeltaX, MouseDeltaY);

		// While ImGui owns the cursor, the scripthook suppresses the game's SetCursorPos
		// recentring, which corrupts the game's delta tracking (deltas become offsets from
		// the screen centre). Ignore the mouse for those frames and for one update after
		// control returns, so the garbage drains without kicking the camera. Deltas too
		// large to be a real hand movement are discarded for the same reason.
		const bool bMouseDeltaSane = (std::fabs(MouseDeltaX) <= FREECAM_MOUSE_DELTA_MAX) && (std::fabs(MouseDeltaY) <= FREECAM_MOUSE_DELTA_MAX);
		if (bCursorCaptured || m_bSwallowMouseDelta || !bMouseDeltaSane)
		{
			MouseDeltaX = 0.0f;
			MouseDeltaY = 0.0f;
			m_bSwallowMouseDelta = bCursorCaptured;
		}

		// Optional mouse smoothing: smooth the mouse *velocity* (counts/sec) rather than the
		// per-frame displacement, then consume velocity * delta time, so the result is
		// frame-rate independent. Because the velocity eases towards zero after the hand
		// stops, the camera coasts out of a pan instead of stopping dead.
		float AppliedMouseDeltaX = MouseDeltaX;
		float AppliedMouseDeltaY = MouseDeltaY;
		if (ACTIVE_SETTINGS.m_bMouseSmoothing && DeltaTime > 0.0f)
		{
			m_SmoothedMouseVelocityX = SmoothTowards(m_SmoothedMouseVelocityX, MouseDeltaX / DeltaTime, ACTIVE_SETTINGS.m_MouseSmoothTime, DeltaTime);
			m_SmoothedMouseVelocityY = SmoothTowards(m_SmoothedMouseVelocityY, MouseDeltaY / DeltaTime, ACTIVE_SETTINGS.m_MouseSmoothTime, DeltaTime);

			// Snap the tiny exponential tail to zero so the camera doesn't creep forever.
			// One count/sec is far below anything a hand or sensor produces.
			if (std::fabs(m_SmoothedMouseVelocityX) < 1.0f)
			{
				m_SmoothedMouseVelocityX = 0.0f;
			}
			if (std::fabs(m_SmoothedMouseVelocityY) < 1.0f)
			{
				m_SmoothedMouseVelocityY = 0.0f;
			}

			AppliedMouseDeltaX = m_SmoothedMouseVelocityX * DeltaTime;
			AppliedMouseDeltaY = m_SmoothedMouseVelocityY * DeltaTime;
		}
		else
		{
			// Keep the state clean while smoothing is off, so toggling it on mid-flight
			// doesn't replay stale velocity.
			m_SmoothedMouseVelocityX = 0.0f;
			m_SmoothedMouseVelocityY = 0.0f;
		}

		const bool bMouseLook = (AppliedMouseDeltaX != 0.0f || AppliedMouseDeltaY != 0.0f);

		if (bMouseLook)
		{
			const float MouseRotateSpeed = FREECAM_MOUSE_ROTATE_SPEED * ACTIVE_SETTINGS.m_MouseSensitivity;

			Yaw -= AppliedMouseDeltaX * MouseRotateSpeed;

			// Pitch follows the game's "Mouse.Invert" option, like the game's own cameras do.
			const float PitchDelta = AppliedMouseDeltaY * MouseRotateSpeed;
			Pitch = std::clamp(IsMouseInvertEnabled() ? Pitch - PitchDelta : Pitch + PitchDelta, -FREECAM_PITCH_LIMIT, FREECAM_PITCH_LIMIT);
		}

		// Right stick -> yaw/pitch rotation. Right stick X drives yaw, right stick Y drives pitch.
		const float YawTarget = bMouseLook ? 0.0f : ApplyStickDeadzone(RightStickX, FREECAM_STICK_DEADZONE);
		const float PitchTarget = bMouseLook ? 0.0f : ApplyStickDeadzone(RightStickY, FREECAM_STICK_DEADZONE);

		if (ACTIVE_SETTINGS.m_bGamepadRotationSmoothing)
		{
			// Smooth towards the target input: fast attack when the input is growing, slower release
			// when it's shrinking, so the camera accelerates in and coasts to a stop.
			const float YawTau = (std::fabs(YawTarget) > std::fabs(m_SmoothedYawInput)) ? ACTIVE_SETTINGS.m_RotationSmoothRampUp : ACTIVE_SETTINGS.m_RotationSmoothDecay;
			const float PitchTau = (std::fabs(PitchTarget) > std::fabs(m_SmoothedPitchInput)) ? ACTIVE_SETTINGS.m_RotationSmoothRampUp : ACTIVE_SETTINGS.m_RotationSmoothDecay;

			m_SmoothedYawInput = SmoothTowards(m_SmoothedYawInput, YawTarget, YawTau, DeltaTime);
			m_SmoothedPitchInput = SmoothTowards(m_SmoothedPitchInput, PitchTarget, PitchTau, DeltaTime);

			// Snap the tiny exponential tail to zero so the camera doesn't creep forever.
			if (std::fabs(m_SmoothedYawInput) < 1e-4f)
			{
				m_SmoothedYawInput = 0.0f;
			}
			if (std::fabs(m_SmoothedPitchInput) < 1e-4f)
			{
				m_SmoothedPitchInput = 0.0f;
			}
		}
		else
		{
			// Smoothing disabled: the stick drives rotation speed directly.
			m_SmoothedYawInput = YawTarget;
			m_SmoothedPitchInput = PitchTarget;
		}

		if (m_SmoothedYawInput != 0.0f)
		{
			Yaw -= m_SmoothedYawInput * ACTIVE_SETTINGS.m_RotateSpeed * DeltaTime;
		}

		// Clamp pitch so the camera can't flip past vertical.
		if (m_SmoothedPitchInput != 0.0f)
		{
			Pitch = std::clamp(Pitch + (m_SmoothedPitchInput * ACTIVE_SETTINGS.m_RotateSpeed * DeltaTime), -FREECAM_PITCH_LIMIT, FREECAM_PITCH_LIMIT);
		}

		// Left stick Y -> forwards/backwards along the look direction (yaw + pitch),
		// left stick X -> strafe along the flat right vector (yaw only).
		const float MoveInput = ApplyStickDeadzone(LeftStickY, FREECAM_STICK_DEADZONE);
		const float StrafeInput = ApplyStickDeadzone(LeftStickX, FREECAM_STICK_DEADZONE);
		if (MoveInput != 0.0f || StrafeInput != 0.0f)
		{
			// NB: Uses XMScalarSinCos in XBOX
			const float SinYaw = std::sin(Yaw);
			const float CosYaw = std::cos(Yaw);
			const float SinPitch = std::sin(Pitch);
			const float CosPitch = std::cos(Pitch);

			const float MoveAmount = MoveInput * ACTIVE_SETTINGS.m_MoveSpeed * SpeedModifier * DeltaTime;
			const float StrafeAmount = StrafeInput * ACTIVE_SETTINGS.m_MoveSpeed * SpeedModifier * DeltaTime;

			m_CameraData.m_Position[0] -= (SinYaw * CosPitch * MoveAmount) + (CosYaw * StrafeAmount);
			m_CameraData.m_Position[1] += SinPitch * MoveAmount;
			m_CameraData.m_Position[2] += (SinYaw * StrafeAmount) - (CosYaw * CosPitch * MoveAmount);
		}
	}

	MarketingCameraSettings& MarketingCamera::GetCameraSettings()
	{
		return ACTIVE_SETTINGS;
	}
}
