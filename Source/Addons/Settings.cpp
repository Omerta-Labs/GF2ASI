#include "Addons/Settings.h"

#include <filesystem>

void Settings::Init()
{
	// TODO: Could probably move this to utility header
	const std::filesystem::path SettingsPath = "scripts/gf2asi.ini";
	const std::filesystem::path GamePath = std::filesystem::current_path();
	const std::filesystem::path CompletePath = GamePath / SettingsPath;

	// file must exist to load .ini properly
	if (std::filesystem::exists(CompletePath))
	{
		const std::wstring& WidePath = CompletePath.wstring();
		ShowModMenuWindowInput = GetPrivateProfileIntW(L"Keybinds", L"model", ShowModMenuWindowInput, WidePath.data());
		FlyModeUpInput = GetPrivateProfileIntW(L"Keybinds", L"flyup", FlyModeUpInput, WidePath.data());
		FlyModeDownInput = GetPrivateProfileIntW(L"Keybinds", L"flydown", FlyModeDownInput, WidePath.data());
	}
}

int Settings::GetShowModMenuWindowInput() const
{
	return ShowModMenuWindowInput;
}

int Settings::GetFlyModeUpInput() const
{
	return FlyModeUpInput;
}

int Settings::GetFlyModeDownInput() const
{
	return FlyModeDownInput;
}
