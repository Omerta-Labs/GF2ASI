#include "Addons/Settings.h"

#include "tConsole.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Debug/MarketingDebug.h"

// C++
#include <algorithm>
#include <cmath>
#include <cwchar>
#include <filesystem>
#include <vector>

namespace
{
	// Everything that is not a key binding
	constexpr wchar_t SETTINGS_FILE_NAME[] = L"gf2asi_settings.ini";

	// Key bindings only, shared with KeybindManager
	constexpr wchar_t KEYBINDS_FILE_NAME[] = L"gf2asi_keybinds.ini";

	// The single config file used before the split, read once to seed the two above
	constexpr wchar_t LEGACY_FILE_NAME[] = L"gf2asi.ini";

	constexpr wchar_t KEYBINDS_SECTION[] = L"Keybinds";

	// Resolve "<game folder>/scripts/<InFileName>".
	// Uses the primitive module file name because otherwise std::filesystem produces bad
	// results, for example a doubled up scripts folder in the path.
	// TODO: Could probably move this to utility header
	std::wstring ResolveScriptsFilePath(const wchar_t* InFileName)
	{
		wchar_t RawExeBuffer[MAX_PATH] = {};
		GetModuleFileNameW(nullptr, RawExeBuffer, ARRAYSIZE(RawExeBuffer));

		static const std::filesystem::path SCRIPTS_FOLDER_NAME = "scripts";

		const std::filesystem::path ExecutablePath = RawExeBuffer;
		return (ExecutablePath.parent_path() / SCRIPTS_FOLDER_NAME / InFileName).wstring();
	}

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

	// Copy one whole section between ini files. GetPrivateProfileSectionW hands the section
	// back as a double null terminated run of "key=value" entries, which is the same shape
	// WritePrivateProfileSectionW takes, so the pair moves a section without this code
	// needing to know which keys are in it.
	bool CopyProfileSection(const wchar_t* InSection, const wchar_t* InSourcePath, const wchar_t* InDestPath)
	{
		// GetPrivateProfileSectionW truncates silently rather than reporting the size it
		// wanted, so start at the 32767 character cap the API tops out at anyway.
		std::vector<wchar_t> SectionBuffer(32767);
		if (GetPrivateProfileSectionW(InSection, SectionBuffer.data(), static_cast<DWORD>(SectionBuffer.size()), InSourcePath) == 0)
		{
			return false;
		}

		// Keep only real key=value pairs. The config files use // comment lines, which the
		// profile API hands back verbatim but cannot parse again on the way in.
		std::vector<wchar_t> Entries;
		for (const wchar_t* Entry = SectionBuffer.data(); *Entry != L'\0'; Entry += std::wcslen(Entry) + 1)
		{
			if (std::wcschr(Entry, L'=') == nullptr)
			{
				continue;
			}

			Entries.insert(Entries.end(), Entry, Entry + std::wcslen(Entry) + 1);
		}

		if (Entries.empty())
		{
			return false;
		}

		// Second terminator, closing the double null terminated list
		Entries.push_back(L'\0');

		return WritePrivateProfileSectionW(InSection, Entries.data(), InDestPath) != FALSE;
	}
}

void Settings::Init()
{
	tConsole::fWriteLine("Settings::Init");

	// Cache the resolved paths even if the files don't exist yet - saving creates them.
	SettingsFilePath = ResolveScriptsFilePath(SETTINGS_FILE_NAME);
	KeybindsFilePath = ResolveScriptsFilePath(KEYBINDS_FILE_NAME);

	MigrateLegacyConfigFile();

	// file must exist to load .ini properly
	tConsole::fPrintf("Attempting to load settings from \"%s\"", std::filesystem::path(SettingsFilePath).string().data());
	if (std::filesystem::exists(SettingsFilePath))
	{
		tConsole::fWriteLine("Loading Settings");

		const std::wstring& WidePath = SettingsFilePath;
		bWantsPreOrderBonus = GetPrivateProfileIntW(L"Mods", L"UnlockPreOrderCrew", true, WidePath.data());
		bApplyCPUFix = GetPrivateProfileIntW(L"Fixes", L"ApplyCpuFix", true, WidePath.data());
		bApplyBrightnessFix = GetPrivateProfileIntW(L"Fixes", L"ApplyBrightnessFix", true, WidePath.data());
		bApplyVSyncFix = GetPrivateProfileIntW(L"Fixes", L"ApplyVSyncFix", bApplyVSyncFix, WidePath.data()) != 0;

		// Held to the same range the game's own options code clamps to
		GammaContrast = GetPrivateProfileFloatW(L"Fixes", L"GammaContrast", GammaContrast, WidePath.data());
		GammaContrast = std::clamp(GammaContrast, 0.8f, 1.2f);

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

		// Mobface head shot capture size, patched in by SH_MugShotFix. The
		// range check lives with the patch, so an out of range value still gets
		// reported once rather than silently corrected here.
		MugShot.bEnable = GetPrivateProfileIntW(L"MugShot", L"Enable", MugShot.bEnable, WidePath.data()) != 0;
		MugShot.Resolution = GetPrivateProfileIntW(L"MugShot", L"Resolution", MugShot.Resolution, WidePath.data());

		tConsole::fPrintf("Wants Pre-Order: %u", bWantsPreOrderBonus);
		tConsole::fPrintf("Wants CPU fix: %u", bApplyCPUFix);
		tConsole::fPrintf("Wants Brightness fix: %u", bApplyBrightnessFix);
		tConsole::fPrintf("Wants VSync fix: %u", bApplyVSyncFix);
		tConsole::fPrintf("Gamma contrast: %.2f", GammaContrast);
		tConsole::fPrintf("EdgeAA enhance: %u (BlurWidth=%.2f, SampleLength=%.2f, Barrier=%.2f, DepthWeight=%g, Deferred=%u)",
			EdgeAA.bEnable, EdgeAA.BlurWidth, EdgeAA.SampleLength, EdgeAA.Barrier, EdgeAA.DepthWeight, EdgeAA.bDeferredMode);
		tConsole::fPrintf("MugShot head shot: %u (Resolution=%u)", MugShot.bEnable, MugShot.Resolution);
		tConsole::fPrintf("PhotoMode Camera: MoveSpeed=%.2f, Boost=x%.2f, RotateSpeed=%.2f rad/s, StickSmooth=%u (RampUp=%.2fs, Decay=%.2fs), MouseSens=x%.2f, MouseSmooth=%u (%.2fs)",
			CurrentSettings.m_MoveSpeed, CurrentSettings.m_MoveSpeedModifier, CurrentSettings.m_RotateSpeed,
			CurrentSettings.m_bGamepadRotationSmoothing, CurrentSettings.m_RotationSmoothRampUp, CurrentSettings.m_RotationSmoothDecay,
			CurrentSettings.m_MouseSensitivity, CurrentSettings.m_bMouseSmoothing, CurrentSettings.m_MouseSmoothTime);
	}
	else
	{
		tConsole::fWriteLine("Settings file missing, not loading and using defaults instead");
	}

	// The fly mode keys live in the keybinds file with the rest of the bindings, but are
	// read here rather than registered with KeybindManager because they are held rather
	// than pressed - see PlayerMasterSM_Modded. A missing file is fine, the profile API
	// hands back the defaults, and KeybindManager writes the file on the first save.
	tConsole::fPrintf("Attempting to load keybinds from \"%s\"", std::filesystem::path(KeybindsFilePath).string().data());
	FlyModeUpInput = GetPrivateProfileIntW(KEYBINDS_SECTION, L"flyup", FlyModeUpInput, KeybindsFilePath.data());
	FlyModeDownInput = GetPrivateProfileIntW(KEYBINDS_SECTION, L"flydown", FlyModeDownInput, KeybindsFilePath.data());

	tConsole::fPrintf("Fly Mode Up Input: 0x%X", FlyModeUpInput);
	tConsole::fPrintf("Fly Mode Down Input: 0x%X", FlyModeDownInput);
}

void Settings::MigrateLegacyConfigFile() const
{
	const std::wstring LegacyFilePath = ResolveScriptsFilePath(LEGACY_FILE_NAME);
	if (!std::filesystem::exists(LegacyFilePath))
	{
		return;
	}

	// Only fill in files that aren't there yet, so a user who has already moved across
	// (or hand authored the new files) never has them overwritten by a stale legacy copy.
	const bool bNeedsSettings = !std::filesystem::exists(SettingsFilePath);
	const bool bNeedsKeybinds = !std::filesystem::exists(KeybindsFilePath);
	if (!bNeedsSettings && !bNeedsKeybinds)
	{
		return;
	}

	tConsole::fPrintf("Migrating pre-split config \"%s\"", std::filesystem::path(LegacyFilePath).string().data());

	if (bNeedsKeybinds && CopyProfileSection(KEYBINDS_SECTION, LegacyFilePath.data(), KeybindsFilePath.data()))
	{
		tConsole::fWriteLine("Migrated [Keybinds] into the keybinds file");
	}

	if (bNeedsSettings)
	{
		std::error_code CopyError;
		std::filesystem::copy_file(LegacyFilePath, SettingsFilePath, CopyError);
		if (CopyError)
		{
			tConsole::fPrintf("Failed to migrate settings: %s", CopyError.message().data());
			return;
		}

		// The bindings have their own file now, so drop the copy that came across with
		// everything else - two sources of truth would silently disagree after a rebind.
		// A null key name deletes the whole section.
		WritePrivateProfileStringW(KEYBINDS_SECTION, nullptr, nullptr, SettingsFilePath.data());

		tConsole::fWriteLine("Migrated settings into the settings file");
	}

	// The profile API caches the files it has touched, and the loads that follow in this
	// same launch read the two files we just wrote. A null section, key and value flushes
	// that cache, so those reads see the migrated contents rather than a stale miss.
	WritePrivateProfileStringW(nullptr, nullptr, nullptr, SettingsFilePath.data());
	WritePrivateProfileStringW(nullptr, nullptr, nullptr, KeybindsFilePath.data());

	// The legacy file is deliberately left on disk. It is the user's data, and keeping it
	// means the migration can be redone by deleting the new files if it went wrong.
}

void Settings::SaveCameraSettings() const
{
	if (SettingsFilePath.empty())
	{
		tConsole::fWriteLine("SaveCameraSettings called before Init, ignoring");
		return;
	}

	const EARS::Modules::MarketingCameraSettings& CurrentSettings = EARS::Modules::MarketingCamera::GetCameraSettings();
	WritePrivateProfileFloatW(L"PhotoMode", L"MoveSpeed", CurrentSettings.m_MoveSpeed, SettingsFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"MoveSpeedModifier", CurrentSettings.m_MoveSpeedModifier, SettingsFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"RotateSpeed", CurrentSettings.m_RotateSpeed, SettingsFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"RotationSmoothRampUp", CurrentSettings.m_RotationSmoothRampUp, SettingsFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"RotationSmoothDecay", CurrentSettings.m_RotationSmoothDecay, SettingsFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"MouseSensitivity", CurrentSettings.m_MouseSensitivity, SettingsFilePath.data());
	WritePrivateProfileStringW(L"PhotoMode", L"GamepadRotationSmoothing", CurrentSettings.m_bGamepadRotationSmoothing ? L"1" : L"0", SettingsFilePath.data());
	WritePrivateProfileStringW(L"PhotoMode", L"MouseSmoothing", CurrentSettings.m_bMouseSmoothing ? L"1" : L"0", SettingsFilePath.data());
	WritePrivateProfileFloatW(L"PhotoMode", L"MouseSmoothTime", CurrentSettings.m_MouseSmoothTime, SettingsFilePath.data());
}

int Settings::GetFlyModeUpInput() const
{
	return FlyModeUpInput;
}

int Settings::GetFlyModeDownInput() const
{
	return FlyModeDownInput;
}
