#include "Addons/Settings.h"

void Settings::Init()
{
	ShowModMenuWindowInput = GetPrivateProfileInt("Keybinds", "model", ShowModMenuWindowInput, "./scripts/gf2asi.ini");
	ShowImGuiDemoWindowInput = GetPrivateProfileInt("Keybinds", "demo", ShowImGuiDemoWindowInput, "./scripts/gf2asi.ini");
	FlyModeUpInput = GetPrivateProfileInt("Keybinds", "flyup", FlyModeUpInput, "./scripts/gf2asi.ini");
	FlyModeDownInput = GetPrivateProfileInt("Keybinds", "flydown", FlyModeDownInput, "./scripts/gf2asi.ini");
}
