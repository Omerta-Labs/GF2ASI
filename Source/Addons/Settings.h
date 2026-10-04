#pragma once

#include "Utils/Singleton.h"

// C++
#include <windows.h>

/**
 * The mod's own settings, and the one-time seeding of the config files.
 *
 * Tuning that belongs to a single fix is not here: each fix reads its own
 * section of gf2asi_settings.ini through Mod::Config::SettingsFile() when it
 * installs, so adding a fix does not touch this class. What is left is the
 * handful of settings with no single owner.
 */
class Settings : public SH::Singleton<Settings>
{
public:

	/**
	 * Seeds the config files from a pre-split gf2asi.ini if one is there, then
	 * reads the settings below.
	 *
	 * Runs before any hook is installed, so a fix reading its own tuning at
	 * install time always sees migrated contents.
	 */
	void Init();

	int GetFlyModeUpInput() const { return FlyModeUpInput; }
	int GetFlyModeDownInput() const { return FlyModeDownInput; }
	bool WantsPreOrderBonus() const { return bWantsPreOrderBonus; }
	bool ApplyCPUFix() const { return bApplyCPUFix; }

private:

	/**
	 * Seed the split config files from a pre-split gf2asi.ini, so an existing install
	 * keeps its settings and key bindings across the rename. Only fills in files that
	 * are not there yet, and leaves the legacy file on disk untouched.
	 */
	void MigrateLegacyConfigFile() const;

	// Virtual Key to get up in fly mode, read from [Keybinds] flyup in the keybinds file.
	// Held rather than pressed, which is why it is not a KeybindManager ShortcutAction.
	int FlyModeUpInput = VK_PRIOR;

	// Virtual Key to get down in fly mode, read from [Keybinds] flydown alongside the above
	int FlyModeDownInput = VK_NEXT;

	// Whether or not the Player wants Pre-order bonus unlocked
	bool bWantsPreOrderBonus = true;

	// Whether or not we should try and limit CPU count
	bool bApplyCPUFix = true;
};
