#include "Addons/Settings.h"

#include "Addons/ConfigFile.h"
#include "tConsole.h"

// C++
#include <filesystem>

namespace
{
	// The single config file used before the split, read once to seed the two
	// files in use now
	constexpr wchar_t LEGACY_FILE_NAME[] = L"gf2asi.ini";

	constexpr wchar_t KEYBINDS_SECTION[] = L"Keybinds";
}

void Settings::Init()
{
	tConsole::fWriteLine("Settings::Init");

	MigrateLegacyConfigFile();

	const Mod::ConfigFile& SettingsFile = Mod::Config::SettingsFile();
	const Mod::ConfigFile& KeybindsFile = Mod::Config::KeybindsFile();

	tConsole::fPrintf("Attempting to load settings from \"%s\"",
		std::filesystem::path(SettingsFile.GetPath()).string().data());

	if (SettingsFile.Exists())
	{
		bWantsPreOrderBonus = SettingsFile.GetBool(L"Mods", L"UnlockPreOrderCrew", bWantsPreOrderBonus);
		bApplyCPUFix = SettingsFile.GetBool(L"Fixes", L"ApplyCpuFix", bApplyCPUFix);

		tConsole::fPrintf("Wants Pre-Order: %u", bWantsPreOrderBonus);
		tConsole::fPrintf("Wants CPU fix: %u", bApplyCPUFix);
	}
	else
	{
		tConsole::fWriteLine("Settings file missing, not loading and using defaults instead");
	}

	// The fly mode keys live in the keybinds file with the rest of the bindings, but are
	// read here rather than registered with KeybindManager because they are held rather
	// than pressed - see PlayerMasterSM_Modded. A missing file is fine, the profile API
	// hands back the defaults, and KeybindManager writes the file on the first save.
	FlyModeUpInput = KeybindsFile.GetInt(KEYBINDS_SECTION, L"flyup", FlyModeUpInput);
	FlyModeDownInput = KeybindsFile.GetInt(KEYBINDS_SECTION, L"flydown", FlyModeDownInput);

	tConsole::fPrintf("Fly Mode Up Input: 0x%X", FlyModeUpInput);
	tConsole::fPrintf("Fly Mode Down Input: 0x%X", FlyModeDownInput);
}

void Settings::MigrateLegacyConfigFile() const
{
	const Mod::ConfigFile LegacyFile(Mod::ResolveScriptsFilePath(LEGACY_FILE_NAME));
	if (!LegacyFile.Exists())
	{
		return;
	}

	Mod::ConfigFile& SettingsFile = Mod::Config::SettingsFile();
	Mod::ConfigFile& KeybindsFile = Mod::Config::KeybindsFile();

	// Only fill in files that aren't there yet, so a user who has already moved across
	// (or hand authored the new files) never has them overwritten by a stale legacy copy.
	const bool bNeedsSettings = !SettingsFile.Exists();
	const bool bNeedsKeybinds = !KeybindsFile.Exists();
	if (!bNeedsSettings && !bNeedsKeybinds)
	{
		return;
	}

	tConsole::fPrintf("Migrating pre-split config \"%s\"",
		std::filesystem::path(LegacyFile.GetPath()).string().data());

	if (bNeedsKeybinds && KeybindsFile.CopySectionFrom(LegacyFile, KEYBINDS_SECTION))
	{
		tConsole::fWriteLine("Migrated [Keybinds] into the keybinds file");
	}

	if (bNeedsSettings)
	{
		std::error_code CopyError;
		std::filesystem::copy_file(LegacyFile.GetPath(), SettingsFile.GetPath(), CopyError);
		if (CopyError)
		{
			tConsole::fPrintf("Failed to migrate settings: %s", CopyError.message().data());
			return;
		}

		// The bindings have their own file now, so drop the copy that came across with
		// everything else - two sources of truth would silently disagree after a rebind.
		SettingsFile.DeleteSection(KEYBINDS_SECTION);

		tConsole::fWriteLine("Migrated settings into the settings file");
	}

	// The loads that follow in this same launch read the two files just written, so the
	// profile API's cached copies have to go or those reads see a stale miss.
	SettingsFile.DropApiCache();
	KeybindsFile.DropApiCache();

	// The legacy file is deliberately left on disk. It is the user's data, and keeping it
	// means the migration can be redone by deleting the new files if it went wrong.
}
