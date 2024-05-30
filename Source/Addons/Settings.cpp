#include "Addons/Settings.h"

void Settings::Init()
{
	LPTSTR path = (LPTSTR) "./scripts/gf2asi.ini";
	ShowModMenuWindowInput = GetPrivateProfileInt("Keybinds", "model", ShowModMenuWindowInput, path);
	ShowImGuiDemoWindowInput = GetPrivateProfileInt("Keybinds", "demo", ShowImGuiDemoWindowInput, path);
	FlyModeUpInput = GetPrivateProfileInt("Keybinds", "flyup", FlyModeUpInput, path);
	FlyModeDownInput = GetPrivateProfileInt("Keybinds", "flydown", FlyModeDownInput, path);
}

int Settings::GetShowModMenuWindowInput()
{
	return ShowModMenuWindowInput;
}

int Settings::GetShowImGuiDemoWindowInput()
{
	return ShowImGuiDemoWindowInput;
}

int Settings::GetFlyModeUpInput()
{
	return FlyModeUpInput;
}

int Settings::GetFlyModeDownInput()
{
	return FlyModeDownInput;
}
