#include "Addons/Settings.h"

void Settings::Init()
{
	LPTSTR path = (LPTSTR) "./scripts/gf2asi.ini";
	ShowModMenuWindowInput = GetPrivateProfileInt("Keybinds", "model", ShowModMenuWindowInput, path);
	ShowImGuiDemoWindowInput = GetPrivateProfileInt("Keybinds", "demo", ShowImGuiDemoWindowInput, path);
	FlyModeUpInput = GetPrivateProfileInt("Keybinds", "flyup", FlyModeUpInput, path);
	FlyModeDownInput = GetPrivateProfileInt("Keybinds", "flydown", FlyModeDownInput, path);
}

int Settings::GetShowModMenuWindowInput() const
{
	return ShowModMenuWindowInput;
}

int Settings::GetShowImGuiDemoWindowInput() const
{
	return ShowImGuiDemoWindowInput;
}

int Settings::GetFlyModeUpInput() const
{
	return FlyModeUpInput;
}

int Settings::GetFlyModeDownInput() const
{
	return FlyModeDownInput;
}
