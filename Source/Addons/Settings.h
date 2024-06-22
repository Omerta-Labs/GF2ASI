#pragma once

#include <windows.h>

class Settings
{
public:

	void Init();

	int GetShowModMenuWindowInput() const;
	int GetFlyModeUpInput() const;
	int GetFlyModeDownInput() const;

private:

	// Virtual Key to show the ImGui Demo Window
	int ShowModMenuWindowInput = VK_F1;

	// Virtual Key to get up in fly mode
	int FlyModeUpInput = VK_PRIOR;

	// Virtual Key to get down in fly mode
	int FlyModeDownInput = VK_NEXT;
};
