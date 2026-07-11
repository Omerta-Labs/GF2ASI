#include "Addons/Settings.h"

#include "tConsole.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Debug/MarketingDebug.h"

// C++
#include <cmath>
#include <cwchar>
#include <filesystem>

namespace
{
	// The Win32 profile API has no float variant, so read the raw string and parse it,
	// keeping the default on missing, malformed or non-finite values.
	float GetPrivateProfileFloatW(const wchar_t* InSection, const wchar_t* InKey, const float InDefaultValue, const wchar_t* InFilePath)
	{
		wchar_t ValueBuffer[64] = {};
		GetPrivateProfileStringW(InSection, InKey, L"", ValueBuffer, ARRAYSIZE(ValueBuffer), InFilePath);

		wchar_t* ParseEnd = nullptr;
		const float ParsedValue = std::wcstof(ValueBuffer, &ParseEnd);
		if (ParseEnd == ValueBuffer || !std::isfinite(ParsedValue))
		{
			return InDefaultValue;
		}

		return ParsedValue;
	}

	// Write counterpart of GetPrivateProfileFloatW: formats the float as text, since the
	// Win32 profile API only stores strings. %g keeps the output compact ("12", "0.08")
	// while round-tripping through wcstof on the next load.
	void WritePrivateProfileFloatW(const wchar_t* InSection, const wchar_t* InKey, const float InValue, const wchar_t* InFilePath)
	{
		wchar_t ValueBuffer[64] = {};
		swprintf_s(ValueBuffer, L"%.6g", InValue);

		WritePrivateProfileStringW(InSection, InKey, ValueBuffer, InFilePath);
	}
}

void Settings::Init()
{
	tConsole::fWriteLine("Settings::Init");

	static const std::filesystem::path SCRIPTS_FOLDER_NAME = "scripts";
	static const std::filesystem::path CONFIG_FILE_NAME = "gf2asi.ini";

	// use primitive module file name because otherwise std::filesystem produces bad results
	// for example double'd up scripts folder in path.
	wchar_t RawExeBuffer[256];
	memset(RawExeBuffer, 0, 256);
	GetModuleFileNameW(nullptr, RawExeBuffer, 256);

	// TODO: Could probably move this to utility header
	const std::filesystem::path ExecutablePath = RawExeBuffer;
	const std::filesystem::path CompletePath = (ExecutablePath.parent_path() / SCRIPTS_FOLDER_NAME / CONFIG_FILE_NAME);

	// Cache the resolved path even if the file doesn't exist yet - saving creates it.
	ConfigFilePath = CompletePath.wstring();

	// file must exist to load .ini properly
	tConsole::fPrintf("Attempting to load settings from \"%s\"", CompletePath.string().data());
	if (std::filesystem::exists(CompletePath))
	{
		tConsole::fWriteLine("Loading Settings");

		const std::wstring& WidePath = ConfigFilePath;
		FlyModeUpInput = GetPrivateProfileIntW(L"Keybinds", L"flyup", VK_PRIOR, WidePath.data());
		FlyModeDownInput = GetPrivateProfileIntW(L"Keybinds", L"flydown", VK_NEXT, WidePath.data());
		bWantsPreOrderBonus = GetPrivateProfileIntW(L"Mods", L"UnlockPreOrderCrew", true, WidePath.data());
		bApplyCPUFix = GetPrivateProfileIntW(L"Fixes", L"ApplyCpuFix", true, WidePath.data());
		bApplyBrightnessFix = GetPrivateProfileIntW(L"Fixes", L"ApplyBrightnessFix", true, WidePath.data());

		// Photo mode camera tuning. The struct's member initialisers are the defaults, so a
		// missing key leaves that value at its factory setting. Angular values are radians.
		EARS::Modules::MarketingCameraSettings& CurrentSettings = EARS::Modules::MarketingCamera::GetCameraSettings();
		CurrentSettings.m_MoveSpeed = GetPrivateProfileFloatW(L"PhotoMode", L"MoveSpeed", CurrentSettings.m_MoveSpeed, WidePath.data());
		CurrentSettings.m_MoveSpeedModifier = GetPrivateProfileFloatW(L"PhotoMode", L"MoveSpeedModifier", CurrentSettings.m_MoveSpeedModifier, WidePath.data());
		CurrentSettings.m_RotateSpeed = GetPrivateProfileFloatW(L"PhotoMode", L"RotateSpeed", CurrentSettings.m_RotateSpeed, WidePath.data());
		CurrentSettings.m_RotationSmoothRampUp = GetPrivateProfileFloatW(L"PhotoMode", L"RotationSmoothRampUp", CurrentSettings.m_RotationSmoothRampUp, WidePath.data());
		CurrentSettings.m_RotationSmoothDecay = GetPrivateProfileFloatW(L"PhotoMode", L"RotationSmoothDecay", CurrentSettings.m_RotationSmoothDecay, WidePath.data());
		CurrentSettings.m_MouseSensitivity = GetPrivateProfileFloatW(L"PhotoMode", L"MouseSensitivity", CurrentSettings.m_MouseSensitivity, WidePath.data());
		CurrentSettings.m_bGamepadRotationSmoothing = GetPrivateProfileIntW(L"PhotoMode", L"GamepadRotationSmoothing", CurrentSettings.m_bGamepadRotationSmoothing, WidePath.data()) != 0;
		CurrentSettings.m_bMouseSmoothing = GetPrivateProfileIntW(L"PhotoMode", L"MouseSmoothing", CurrentSettings.m_bMouseSmoothing, WidePath.data()) != 0;
		CurrentSettings.m_MouseSmoothTime = GetPrivateProfileFloatW(L"PhotoMode", L"MouseSmoothTime", CurrentSettings.m_MouseSmoothTime, WidePath.data());

		// EdgeAA post-process tuning, applied every frame by SH_EdgeAA
		EdgeAA.bEnable = GetPrivateProfileIntW(L"EdgeAA", L"Enable", EdgeAA.bEnable, WidePath.data()) != 0;
		EdgeAA.BlurWidth = GetPrivateProfileFloatW(L"EdgeAA", L"BlurWidth", EdgeAA.BlurWidth, WidePath.data());
		EdgeAA.SampleLength = GetPrivateProfileFloatW(L"EdgeAA", L"SampleLength", EdgeAA.SampleLength, WidePath.data());
		EdgeAA.Cutoff = GetPrivateProfileFloatW(L"EdgeAA", L"Cutoff", EdgeAA.Cutoff, WidePath.data());
		EdgeAA.Barrier = GetPrivateProfileFloatW(L"EdgeAA", L"Barrier", EdgeAA.Barrier, WidePath.data());
		EdgeAA.DepthWeight = GetPrivateProfileFloatW(L"EdgeAA", L"DepthWeight", EdgeAA.DepthWeight, WidePath.data());
		EdgeAA.bDeferredMode = GetPrivateProfileIntW(L"EdgeAA", L"DeferredMode", EdgeAA.bDeferredMode, WidePath.data()) != 0;

		tConsole::fPrintf("Fly Mode Up Input: 0x%X", FlyModeUpInput);
		tConsole::fPrintf("Fly Mode Down Input: 0x%X", FlyModeDownInput);
		tConsole::fPrintf("Wants Pre-Order: %u", bWantsPreOrderBonus);
		tConsole::fPrintf("Wants CPU fix: %u", bApplyCPUFix);
		tConsole::fPrintf("Wants Brightness fix: %u", bApplyBrightnessFix);
		tConsole::fPrintf("EdgeAA enhance: %u (BlurWidth=%.2f, SampleLength=%.2f, Barrier=%.2f, DepthWeight=%g, Deferred=%u)",
			EdgeAA.bEnable, EdgeAA.BlurWidth, EdgeAA.SampleLength, EdgeAA.Barrier, EdgeAA.DepthWeight, EdgeAA.bDeferredMode);
		tConsole::fPrintf("PhotoMode Camera: MoveSpeed=%.2f, Boost=x%.2f, RotateSpeed=%.2f rad/s, StickSmooth=%u (RampUp=%.2fs, Decay=%.2fs), MouseSens=x%.2f, MouseSmooth=%u (%.2fs)",
			CurrentSettings.m_MoveSpeed, CurrentSettings.m_MoveSpeedModifier, CurrentSettings.m_RotateSpeed,
			CurrentSettings.m_bGamepadRotationSmoothing, CurrentSettings.m_RotationSmoothRampUp, CurrentSettings.m_RotationSmoothDecay,
			CurrentSettings.m_MouseSensitivity, CurrentSettings.m_bMouseSmoothing, CurrentSettings.m_MouseSmoothTime);
	}
	else
	{
		tConsole::fWriteLine("Settings file missing, not loading and using defaults instead");
	}
}

void Settings::SaveCameraSettings() const
{
	if (ConfigFilePath.empty())
	{
		tConsole::fWriteLine("SaveCameraSettings called before Init, ignoring");
		return;
	}

	const EARS::Modules::MarketingCameraSettings& CurrentSettings = EARS::Modules::MarketingCamera::GetCameraSettings();
	WritePrivateProfileFloatW(L"PhotoMode", L"MoveSpeed", CurrentSettings.m_MoveSpeed, ConfigFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"MoveSpeedModifier", CurrentSettings.m_MoveSpeedModifier, ConfigFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"RotateSpeed", CurrentSettings.m_RotateSpeed, ConfigFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"RotationSmoothRampUp", CurrentSettings.m_RotationSmoothRampUp, ConfigFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"RotationSmoothDecay", CurrentSettings.m_RotationSmoothDecay, ConfigFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"MouseSensitivity", CurrentSettings.m_MouseSensitivity, ConfigFilePath.data());
	WritePrivateProfileStringW(L"PhotoMode", L"GamepadRotationSmoothing", CurrentSettings.m_bGamepadRotationSmoothing ? L"1" : L"0", ConfigFilePath.data());
	WritePrivateProfileStringW(L"PhotoMode", L"MouseSmoothing", CurrentSettings.m_bMouseSmoothing ? L"1" : L"0", ConfigFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"MouseSmoothTime", CurrentSettings.m_MouseSmoothTime, ConfigFilePath.data());
}

int Settings::GetFlyModeUpInput() const
{
	return FlyModeUpInput;
}

int Settings::GetFlyModeDownInput() const
{
	return FlyModeDownInput;
}
