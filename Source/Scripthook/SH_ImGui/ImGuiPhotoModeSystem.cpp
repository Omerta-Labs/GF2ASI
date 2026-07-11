#include "ImGuiPhotoModeSystem.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/Settings.h"
#include "Addons/imgui/imgui.h"

// Scripthook
#include "Scripthook/SH_ImGui/ImGuiManager.h"
#include "Scripthook/SH_ImGui/ImGuiPropertyGrid.h"

// SDK
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Framework/MainLoop/Logic.h"
#include "SDK/EARS_Framework/Modules/ScreenFX/ScreenFX.h"
#include "SDK/EARS_Godfather/Modules/Debug/MarketingDebug.h"

// C++
#include <cfloat>
#include <cmath>

namespace
{
	constexpr float RADIANS_TO_DEGREES = 57.2957795f;
	constexpr float DEGREES_TO_RADIANS = 0.0174532925f;

	// Slider bounds for the photo mode camera FOV (degrees)
	constexpr float PHOTOMODE_FOV_MIN = 20.0f;
	constexpr float PHOTOMODE_FOV_MAX = 120.0f;

	// Pitch limit matching the free camera's own clamp (~85 degrees)
	constexpr float PHOTOMODE_PITCH_LIMIT = 85.0f;

	// Pause context used when freezing time (same context the debug pause uses)
	constexpr uint32_t PHOTOMODE_PAUSE_CONTEXT = 4;

	// How long screen effects take to ramp in or out when toggled
	constexpr float EFFECT_RAMP_DURATION = 0.35f;

	// Event IDs broadcast by the game to enable/disable the player's controls
	constexpr uintptr_t PLAYER_DISABLE_CONTROLS_EVENT_ID = 0x112B56C;
	constexpr uintptr_t PLAYER_ENABLE_CONTROLS_EVENT_ID = 0x112B39C;

	void SendPlayerControlsEvent(const uintptr_t InEventIdAddress)
	{
		hook::Type<RWS::CEventId> ControlsEventId = hook::Type<RWS::CEventId>(InEventIdAddress);
		MemUtils::CallCdeclMethod<void, RWS::CEventId&, bool>(0x0408A00, ControlsEventId, false);
	}

	// Wrap an angle to [-180, 180] degrees for display; the camera stores unbounded radians
	float WrapDegrees(const float InDegrees)
	{
		return std::remainder(InDegrees, 360.0f);
	}
}

namespace SH
{
	void ImGuiPhotoModeSystem::DrawTab()
	{
		ImGui::BeginChild("photomode_window");
		bool bInPhotoMode = (PhotoModeCameraInfo != nullptr);

		ImGui::TextWrapped("Detach the camera from the player to compose a shot. "
			"Move with the left stick, look with the right stick or mouse, hold LB or left mouse click to move faster.");

		if (!bInPhotoMode)
		{
			if (ImGui::Button("Enter Photo Mode", ImVec2(-FLT_MIN, 0.0f)))
			{
				StartPhotoMode();
			}
		}
		else
		{
			if (ImGui::Button("Exit Photo Mode", ImVec2(-FLT_MIN, 0.0f)))
			{
				StopPhotoMode();
				bInPhotoMode = false;
			}
		}

		ImGui::SeparatorText("Scene");

		ImGui::BeginDisabled(!bInPhotoMode);

		if (BeginPropertyTable("SceneProperties"))
		{
			BeginPropertyRow("Freeze Player", "Disables the player's controls, so they hold their pose while the camera moves.");
			bool bNewFreezePlayer = bPlayerFrozen;
			if (ImGui::Checkbox("##FreezePlayer", &bNewFreezePlayer))
			{
				SetPlayerFrozen(bNewFreezePlayer);
			}

			BeginPropertyRow("Freeze Time", "Pauses gameplay while keeping the photo mode camera free to move.");
			bool bNewFreezeTime = bTimeFrozen;
			if (ImGui::Checkbox("##FreezeTime", &bNewFreezeTime))
			{
				SetTimeFrozen(bNewFreezeTime);
			}

			ImGui::EndTable();
		}

		ImGui::EndDisabled();

		ImGui::SeparatorText("Camera");

		EARS::Modules::MarketingCamera* ActiveCamera = bInPhotoMode ? PhotoModeCameraInfo->GetActiveCamera() : nullptr;
		if (ActiveCamera)
		{
			if (BeginPropertyTable("CameraProperties"))
			{
				RwV3d CameraPosition;
				ActiveCamera->GetPosition(CameraPosition);

				BeginPropertyRow("Position", "Drag to nudge the camera, or double-click a field to type a value.");
				float Position[3] = { CameraPosition.m_X, CameraPosition.m_Y, CameraPosition.m_Z };
				if (ImGui::DragFloat3("##Position", Position, 0.05f, 0.0f, 0.0f, "%.2f"))
				{
					ActiveCamera->SetPosition(RwV3d(Position[0], Position[1], Position[2]));
				}

				RwV3d CameraRotation;
				ActiveCamera->GetRotation(CameraRotation);

				BeginPropertyRow("Yaw", "Turns the camera left and right.");
				float YawDegrees = WrapDegrees(CameraRotation.m_Y * RADIANS_TO_DEGREES);
				if (ImGui::SliderFloat("##Yaw", &YawDegrees, -180.0f, 180.0f, "%.1f deg"))
				{
					CameraRotation.m_Y = YawDegrees * DEGREES_TO_RADIANS;
					ActiveCamera->SetRotation(CameraRotation);
				}

				BeginPropertyRow("Pitch", "Tilts the camera up and down.");
				float PitchDegrees = CameraRotation.m_X * RADIANS_TO_DEGREES;
				if (ImGui::SliderFloat("##Pitch", &PitchDegrees, -PHOTOMODE_PITCH_LIMIT, PHOTOMODE_PITCH_LIMIT, "%.1f deg"))
				{
					CameraRotation.m_X = PitchDegrees * DEGREES_TO_RADIANS;
					ActiveCamera->SetRotation(CameraRotation);
				}

				BeginPropertyRow("Roll", "Tilts the horizon, for dutch-angle shots.");
				float RollDegrees = WrapDegrees(CameraRotation.m_Z * RADIANS_TO_DEGREES);
				if (ImGui::SliderFloat("##Roll", &RollDegrees, -180.0f, 180.0f, "%.1f deg"))
				{
					CameraRotation.m_Z = RollDegrees * DEGREES_TO_RADIANS;
					ActiveCamera->SetRotation(CameraRotation);
				}

				BeginPropertyRow("Field of View", "Camera zoom - lower values zoom in, higher values widen the shot.");

				// Leave room in the value column for the inline reset button
				const ImGuiStyle& Style = ImGui::GetStyle();
				const float ResetButtonWidth = ImGui::CalcTextSize("Reset").x + Style.FramePadding.x * 2.0f;
				ImGui::SetNextItemWidth(-(ResetButtonWidth + Style.ItemInnerSpacing.x));

				float FovDegrees = ActiveCamera->GetFOV() * RADIANS_TO_DEGREES;
				if (ImGui::SliderFloat("##FieldOfView", &FovDegrees, PHOTOMODE_FOV_MIN, PHOTOMODE_FOV_MAX, "%.1f deg"))
				{
					ActiveCamera->SetFOV(FovDegrees * DEGREES_TO_RADIANS);
				}

				ImGui::SameLine(0.0f, Style.ItemInnerSpacing.x);
				if (ImGui::Button("Reset"))
				{
					ActiveCamera->SetFOV(DefaultCameraFOV);
				}
				ImGui::SetItemTooltip("Restore the field of view the game camera had when photo mode was entered.");

				ImGui::EndTable();
			}
		}
		else
		{
			// Not in photo mode - show a read-only summary of the game's active camera
			const EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();

			if (BeginPropertyTable("CameraSummary"))
			{
				RwV3d CameraPosition;
				CameraMgr->GetActiveCamPos(CameraPosition, 0);
				BeginPropertyRow("Position");
				ImGui::Text("%.2f, %.2f, %.2f", CameraPosition.m_X, CameraPosition.m_Y, CameraPosition.m_Z);

				RwV3d CameraRotation;
				CameraMgr->GetActiveCamRot(CameraRotation, 0);
				BeginPropertyRow("Yaw");
				ImGui::Text("%.1f deg", WrapDegrees(CameraRotation.m_Y * RADIANS_TO_DEGREES));

				float Fov = 0.0f;
				CameraMgr->GetActiveCamFov(Fov, 0);
				BeginPropertyRow("Field of View");
				ImGui::Text("%.1f deg", Fov * RADIANS_TO_DEGREES);

				ImGui::EndTable();
			}
		}

		if (ImGui::CollapsingHeader("Camera Feel"))
		{
			ImGui::TextWrapped("Tune how the photo mode camera handles. Changes apply immediately and are saved to the config file.");

			EARS::Modules::MarketingCameraSettings& CameraSettings = EARS::Modules::MarketingCamera::GetCameraSettings();

			// Save once per finished edit (widget release), not on every drag frame
			bool bWantsSave = false;

			if (BeginPropertyTable("CameraFeelProperties"))
			{
				BeginPropertyRow("Movement Speed", "How fast the camera flies at full stick deflection.");
				ImGui::SliderFloat("##MovementSpeed", &CameraSettings.m_MoveSpeed, 1.0f, 100.0f, "%.1f units/s", ImGuiSliderFlags_Logarithmic);
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				BeginPropertyRow("Speed Boost", "Movement speed multiplier while the speed-up button is held.");
				ImGui::SliderFloat("##SpeedBoost", &CameraSettings.m_MoveSpeedModifier, 1.0f, 20.0f, "x%.1f");
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				BeginPropertyRow("Rotation Speed", "How fast the camera turns at full stick deflection. Mouse look is unaffected.");
				float RotateSpeedDegrees = CameraSettings.m_RotateSpeed * RADIANS_TO_DEGREES;
				if (ImGui::SliderFloat("##RotationSpeed", &RotateSpeedDegrees, 30.0f, 360.0f, "%.0f deg/s"))
				{
					CameraSettings.m_RotateSpeed = RotateSpeedDegrees * DEGREES_TO_RADIANS;
				}
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				BeginPropertyRow("Stick Smoothing", "Eases stick rotation in and out instead of starting and stopping instantly.");
				bWantsSave |= ImGui::Checkbox("##StickSmoothing", &CameraSettings.m_bGamepadRotationSmoothing);

				ImGui::BeginDisabled(!CameraSettings.m_bGamepadRotationSmoothing);

				BeginPropertyRow("Rotation Ramp-Up", "Time for stick rotation to reach full speed. Lower feels snappier.");
				ImGui::SliderFloat("##RotationRampUp", &CameraSettings.m_RotationSmoothRampUp, 0.0f, 0.5f, "%.2f s");
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				BeginPropertyRow("Rotation Coast-Out", "Time for the camera to ease to a stop after releasing the stick. Higher feels smoother.");
				ImGui::SliderFloat("##RotationCoastOut", &CameraSettings.m_RotationSmoothDecay, 0.0f, 1.0f, "%.2f s");
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				ImGui::EndDisabled();

				BeginPropertyRow("Mouse Sensitivity", "Mouse look multiplier, on top of the game's own mouse sensitivity option.");
				ImGui::SliderFloat("##MouseSensitivity", &CameraSettings.m_MouseSensitivity, 0.1f, 5.0f, "x%.2f", ImGuiSliderFlags_Logarithmic);
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				BeginPropertyRow("Mouse Smoothing", "Smooths mouse look for buttery cinematic pans. Off by default, because it "
					"makes the camera slightly lag your hand and coast after it stops.");
				bWantsSave |= ImGui::Checkbox("##MouseSmoothing", &CameraSettings.m_bMouseSmoothing);

				ImGui::BeginDisabled(!CameraSettings.m_bMouseSmoothing);

				BeginPropertyRow("Mouse Smooth Time", "How strongly mouse motion is smoothed. Higher is smoother but laggier.");
				ImGui::SliderFloat("##MouseSmoothTime", &CameraSettings.m_MouseSmoothTime, 0.01f, 0.3f, "%.2f s");
				bWantsSave |= ImGui::IsItemDeactivatedAfterEdit();

				ImGui::EndDisabled();

				ImGui::EndTable();
			}

			if (ImGui::Button("Reset to Defaults"))
			{
				CameraSettings = EARS::Modules::MarketingCameraSettings();
				bWantsSave = true;
			}

			if (bWantsSave)
			{
				Settings::GetCheckedRef().SaveCameraSettings();
			}
		}

		DrawEffectsSection(bInPhotoMode);

		ImGui::EndChild();
	}

	void ImGuiPhotoModeSystem::DrawEffectsSection(const bool bInPhotoMode)
	{
		ImGui::SeparatorText("Effects");

		EARS::Modules::ScreenFX* ScreenFXInst = EARS::Modules::ScreenFX::GetInstance();

		ImGui::BeginDisabled(!bInPhotoMode || ScreenFXInst == nullptr);

		if (BeginPropertyTable("EffectProperties"))
		{
			BeginPropertyRow("Letterbox", "Cinematic black bars across the top and bottom of the frame.");
			bool bNewLetterbox = bLetterboxEnabled;
			if (ImGui::Checkbox("##Letterbox", &bNewLetterbox))
			{
				if (bNewLetterbox)
				{
					ScreenFXInst->LetterBoxBegin(EFFECT_RAMP_DURATION);
				}
				else
				{
					ScreenFXInst->LetterBoxEnd(EFFECT_RAMP_DURATION);
				}

				bLetterboxEnabled = bNewLetterbox;
			}

			BeginPropertyRow("Depth of Field", "Blurs the scene based on distance from the focal point, for that portrait look.");
			bool bNewDepthOfField = bDepthOfFieldEnabled;
			if (ImGui::Checkbox("##DepthOfField", &bNewDepthOfField))
			{
				if (bNewDepthOfField)
				{
					ScreenFXInst->Depth_SetFocus(DepthOfFieldFocus);
					ScreenFXInst->Depth_SetIntensity(DepthOfFieldIntensity);
					ScreenFXInst->Depth_Start(EFFECT_RAMP_DURATION);
				}
				else
				{
					ScreenFXInst->Depth_Stop(EFFECT_RAMP_DURATION);
				}

				bDepthOfFieldEnabled = bNewDepthOfField;
			}

			ImGui::BeginDisabled(!bDepthOfFieldEnabled);

			BeginPropertyRow("Focus Distance", "Distance from the camera that stays sharp. Everything nearer or farther blurs.");
			if (ImGui::SliderFloat("##FocusDistance", &DepthOfFieldFocus, 0.5f, 100.0f, "%.1f", ImGuiSliderFlags_Logarithmic))
			{
				ScreenFXInst->Depth_SetFocus(DepthOfFieldFocus);
			}

			BeginPropertyRow("Focus Intensity", "Strength of the out-of-focus blur.");
			if (ImGui::SliderFloat("##FocusIntensity", &DepthOfFieldIntensity, 0.0f, 1.0f, "%.2f"))
			{
				ScreenFXInst->Depth_SetIntensity(DepthOfFieldIntensity);
			}

			ImGui::EndDisabled();

			BeginPropertyRow("Motion Blur", "Smears the frame along camera movement - only visible while the camera is moving. "
				"For a static blur, use Depth of Field instead.");
			bool bNewBlur = bBlurEnabled;
			if (ImGui::Checkbox("##Blur", &bNewBlur))
			{
				if (bNewBlur)
				{
					ScreenFXInst->GetBlurEffect().SetIntensity(BlurIntensity);
					ScreenFXInst->Blur_Start(EFFECT_RAMP_DURATION);
				}
				else
				{
					ScreenFXInst->Blur_Stop(EFFECT_RAMP_DURATION);
				}

				bBlurEnabled = bNewBlur;
			}

			ImGui::BeginDisabled(!bBlurEnabled);

			BeginPropertyRow("Blur Intensity", "Strength of the motion smear while the camera moves.");
			if (ImGui::SliderFloat("##BlurIntensity", &BlurIntensity, 0.0f, 1.0f, "%.2f"))
			{
				ScreenFXInst->GetBlurEffect().SetIntensity(BlurIntensity);
			}

			ImGui::EndDisabled();

			BeginPropertyRow("Bloom", "Makes bright areas glow and bleed into their surroundings. "
				"Blends over the scene's own bloom, so it mostly reads as on/off unless pushed near full intensity.");
			bool bNewBloom = bBloomEnabled;
			if (ImGui::Checkbox("##Bloom", &bNewBloom))
			{
				if (bNewBloom)
				{
					ScreenFXInst->GetBloomEffect().SetIntensity(BloomIntensity);
					ScreenFXInst->Bloom_Start(EFFECT_RAMP_DURATION);
				}
				else
				{
					ScreenFXInst->Bloom_Stop(EFFECT_RAMP_DURATION);
				}

				bBloomEnabled = bNewBloom;
			}

			ImGui::EndTable();
		}

		ImGui::EndDisabled();
	}

	void ImGuiPhotoModeSystem::StopAllEffects()
	{
		EARS::Modules::ScreenFX* ScreenFXInst = EARS::Modules::ScreenFX::GetInstance();
		if (ScreenFXInst)
		{
			if (bLetterboxEnabled)
			{
				ScreenFXInst->LetterBoxEnd(EFFECT_RAMP_DURATION);
			}

			if (bDepthOfFieldEnabled)
			{
				ScreenFXInst->Depth_Stop(EFFECT_RAMP_DURATION);
			}

			if (bBlurEnabled)
			{
				ScreenFXInst->Blur_Stop(EFFECT_RAMP_DURATION);
			}

			if (bBloomEnabled)
			{
				ScreenFXInst->Bloom_Stop(EFFECT_RAMP_DURATION);
			}
		}

		bLetterboxEnabled = false;
		bDepthOfFieldEnabled = false;
		bBlurEnabled = false;
		bBloomEnabled = false;
	}

	void ImGuiPhotoModeSystem::Toggle()
	{
		if (PhotoModeCameraInfo != nullptr)
		{
			StopPhotoMode();
			return;
		}

		// StartPhotoMode dereferences the camera manager, which only exists in-game;
		// ignore the shortcut at the front end rather than crashing.
		if (EARS::Framework::CameraManager::GetInstance() == nullptr)
		{
			return;
		}

		StartPhotoMode();
	}

	void ImGuiPhotoModeSystem::StartPhotoMode()
	{
		EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();

		// Remember the game camera's FOV so "Reset Field of View" can restore it
		CameraMgr->GetActiveCamFov(DefaultCameraFOV, 0);

		uint32_t CompSize = 0;
		void* Mem = EARS::Modules::MarketingCameraInfo::operator new(sizeof(EARS::Modules::MarketingCameraInfo), CompSize);
		PhotoModeCameraInfo = new (Mem) EARS::Modules::MarketingCameraInfo();

		CameraMgr->PushCameraInfo(0, PhotoModeCameraInfo, 0.0f, EARS::Framework::CAMERA_INTERP_NONE, true);

		ImGuiManager::StaticGetUISystemDebug().SupressHUD();
		ImGuiManager::StaticGetUISystemDebug().HideAllNPCIndicators();
		SetPlayerFrozen(true);
		SetTimeFrozen(true);
	}

	void ImGuiPhotoModeSystem::StopPhotoMode()
	{
		EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();
		CameraMgr->PopCameraInfo(0, PhotoModeCameraInfo, 0.0f, EARS::Framework::CAMERA_INTERP_NONE, true, true);

		delete PhotoModeCameraInfo;
		PhotoModeCameraInfo = nullptr;

		// Hand back anything we still hold frozen, and ramp out any active effects
		ImGuiManager::StaticGetUISystemDebug().UnsupressHUD();
		ImGuiManager::StaticGetUISystemDebug().ShowAllNPCIndicators();
		SetPlayerFrozen(false);
		SetTimeFrozen(false);
		StopAllEffects();
	}

	void ImGuiPhotoModeSystem::SetPlayerFrozen(const bool bFreeze)
	{
		if (bFreeze == bPlayerFrozen)
		{
			return;
		}

		SendPlayerControlsEvent(bFreeze ? PLAYER_DISABLE_CONTROLS_EVENT_ID : PLAYER_ENABLE_CONTROLS_EVENT_ID);

		bPlayerFrozen = bFreeze;
	}

	void ImGuiPhotoModeSystem::SetTimeFrozen(const bool bFreeze)
	{
		if (bFreeze == bTimeFrozen)
		{
			return;
		}

		EARS::Framework::CameraManager* CameraMgr = EARS::Framework::CameraManager::GetInstance();

		if (bFreeze)
		{
			RWS::MainLoop::Logic::PushPause(PHOTOMODE_PAUSE_CONTEXT);
			CameraMgr->EnableUpdateWhenPaused(true);
		}
		else
		{
			RWS::MainLoop::Logic::PopPause(PHOTOMODE_PAUSE_CONTEXT);
			CameraMgr->EnableUpdateWhenPaused(false);
		}

		bTimeFrozen = bFreeze;
	}

	void ImGuiPhotoModeSystem::CloseLevelServices()
	{
		if (PhotoModeCameraInfo)
		{
			StopPhotoMode();
		}
	}
}
