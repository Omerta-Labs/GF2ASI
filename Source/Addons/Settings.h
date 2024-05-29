#pragma once
#include <windows.h>

class Settings
{
public:

	void Init();

	// Virtual Key to show the ImGui Demo Window
	int ShowModMenuWindowInput = VK_F1;

	// Virtual Key to show the Parted Model Window
	int ShowImGuiDemoWindowInput = VK_F2;

	// Virtual Key to get up in fly mode
	int FlyModeUpInput = VK_PRIOR;

	// Virtual Key to get down in fly mode
	int FlyModeDownInput = VK_NEXT;
};
