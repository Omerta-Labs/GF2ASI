#include "Addons/Settings.h"

#include "tConsole.h"

#include <filesystem>

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

	// file must exist to load .ini properly
	tConsole::fPrintf("Attempting to load settings from \"%s\"", CompletePath.string().data());
	if (std::filesystem::exists(CompletePath))
	{
		tConsole::fWriteLine("Loading Settings");

		const std::wstring& WidePath = CompletePath.wstring();
		ShowModMenuWindowInput = GetPrivateProfileIntW(L"Keybinds", L"model", VK_F1, WidePath.data());
		ImGuiInteractiveInput = GetPrivateProfileIntW(L"Keybinds", L"ImGuiInteractiveToggle", VK_F2, WidePath.data());
		FlyModeUpInput = GetPrivateProfileIntW(L"Keybinds", L"flyup", VK_PRIOR, WidePath.data());
		FlyModeDownInput = GetPrivateProfileIntW(L"Keybinds", L"flydown", VK_NEXT, WidePath.data());
		bWantsPreOrderBonus = GetPrivateProfileIntW(L"Mods", L"UnlockPreOrderCrew", true, WidePath.data());
		bApplyCPUFix = GetPrivateProfileIntW(L"Fixes", L"ApplyCpuFix", true, WidePath.data());

		tConsole::fPrintf("Show Menu Input: 0x%X", ShowModMenuWindowInput);
		tConsole::fPrintf("ImGui Interactive: 0x%X", ImGuiInteractiveInput);
		tConsole::fPrintf("Fly Mode Up Input: 0x%X", FlyModeUpInput);
		tConsole::fPrintf("Fly Mode Down Input: 0x%X", FlyModeDownInput);
		tConsole::fPrintf("Wants Pre-Order: %u", bWantsPreOrderBonus);
		tConsole::fPrintf("Wants CPU fix: %u", bApplyCPUFix);
	}
	else
	{
		tConsole::fWriteLine("Settings file missing, not loading and using defaults instead");
	}
}

int Settings::GetShowModMenuWindowInput() const
{
	return ShowModMenuWindowInput;
}

int Settings::GetImGuiInteractiveInput() const
{
	return ImGuiInteractiveInput;
}

int Settings::GetFlyModeUpInput() const
{
	return FlyModeUpInput;
}

int Settings::GetFlyModeDownInput() const
{
	return FlyModeDownInput;
}
