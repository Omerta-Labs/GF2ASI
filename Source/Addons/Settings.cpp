#include "Addons/Settings.h"

void Settings::Init()
{
	LPTSTR path = (LPTSTR) "./scripts/gf2asi.ini";
	ShowModMenuWindowInput = GetPrivateProfileInt("Keybinds", "model", ShowModMenuWindowInput, path);
	FlyModeUpInput = GetPrivateProfileInt("Keybinds", "flyup", FlyModeUpInput, path);
	FlyModeDownInput = GetPrivateProfileInt("Keybinds", "flydown", FlyModeDownInput, path);
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
