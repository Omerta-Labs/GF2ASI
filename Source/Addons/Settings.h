#pragma once

#include "Utils/Singleton.h"

// C++
#include <windows.h>

class Settings : public SH::Singleton<Settings>
{
public:

	void Init();

	int GetShowModMenuWindowInput() const;
	int GetImGuiInteractiveInput() const;
	int GetFlyModeUpInput() const;
	int GetFlyModeDownInput() const;
	bool WantsPreOrderBonus() const { return bWantsPreOrderBonus; }
	bool ApplyCPUFix() const { return bApplyCPUFix; }

private:

	// Virtual Key to show the ImGui Demo Window
	int ShowModMenuWindowInput = VK_F1;

	// Virtual Key to show the ImGui Demo Window
	int ImGuiInteractiveInput = VK_F2;

	// Virtual Key to get up in fly mode
	int FlyModeUpInput = VK_PRIOR;

	// Virtual Key to get down in fly mode
	int FlyModeDownInput = VK_NEXT;

	// Whether or not the Player wants Pre-order bonus unlocked
	bool bWantsPreOrderBonus = true;

	// Whether or not we should try and limit CPU count
	bool bApplyCPUFix = true;
};
