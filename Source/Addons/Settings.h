#pragma once

#include "Utils/Singleton.h"

// C++
#include <string>
#include <windows.h>

/**
 * Tuning for the forced EdgeAA post-process (see SH_EdgeAA). The member
 * initialisers are the defaults; the [EdgeAA] section of gf2asi.ini
 * overrides them.
 */
struct EdgeAATuning
{
	// Force the EdgeAA post-process on every frame.
	// Default off until it has been tested enough for release.
	bool bEnable = false;

	// Edge blur strength (uploaded as g_TexelSize.z). The engine skips the
	// pass below 1/127; the shader was authored around values of ~1.0.
	float BlurWidth = 1.0f;

	// Distance in texels between edge-detect samples
	float SampleLength = 1.0f;

	// Edge cutoff, only used by the deferred-mode shader variant
	float Cutoff = 0.0f;

	// Edge detection threshold (g_BarrierAndWeights.x); engine default 0.8.
	// Lower values classify more pixels as edges to be smoothed.
	float Barrier = 0.8f;

	// Depth-difference weight (g_BarrierAndWeights.z); engine default ~7e-6.
	// Higher values make small depth discontinuities register as edges.
	float DepthWeight = 6.9907e-06f;

	// Use the deferred-AA shader variant (normal-buffer edge detect).
	// Experimental - the game data never enables this path on PC.
	bool bDeferredMode = false;
};

class Settings : public SH::Singleton<Settings>
{
public:

	void Init();

	// Resolved path to gf2asi.ini, so other systems (e.g. KeybindManager) can
	// load and persist their own sections against the same file.
	const std::wstring& GetConfigFilePath() const { return ConfigFilePath; }

	int GetFlyModeUpInput() const;
	int GetFlyModeDownInput() const;
	bool WantsPreOrderBonus() const { return bWantsPreOrderBonus; }
	bool ApplyCPUFix() const { return bApplyCPUFix; }
	bool ApplyBrightnessFix() const { return bApplyBrightnessFix; }
	const EdgeAATuning& GetEdgeAATuning() const { return EdgeAA; }

	/**
	 * Persist the current photo mode camera settings to the config file.
	 * Creates the file (and section) if it doesn't exist yet.
	 */
	void SaveCameraSettings() const;

private:

	// Resolved path to the config file, cached by Init() so saves target the same file
	std::wstring ConfigFilePath;

	// Virtual Key to get up in fly mode
	int FlyModeUpInput = VK_PRIOR;

	// Virtual Key to get down in fly mode
	int FlyModeDownInput = VK_NEXT;

	// Whether or not the Player wants Pre-order bonus unlocked
	bool bWantsPreOrderBonus = true;

	// Whether or not we should try and limit CPU count
	bool bApplyCPUFix = true;

	// Whether the brightness/gamma option should work in windowed mode and
	// stop clobbering desktop calibration in fullscreen.
	// Default off until it has been tested enough for release.
	bool bApplyBrightnessFix = false;

	// Tuning for the forced EdgeAA post-process, see SH_EdgeAA
	EdgeAATuning EdgeAA;
};
