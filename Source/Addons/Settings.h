#pragma once

#include "Utils/Singleton.h"

// C++
#include <cstdint>
#include <string>
#include <windows.h>

/**
 * Tuning for the forced EdgeAA post-process (see SH_EdgeAA). The member
 * initialisers are the defaults; the [EdgeAA] section of gf2asi_settings.ini
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

/**
 * Tuning for the mobface head shot capture (see SH_MugShotFix). The member
 * initialisers are the defaults; the [MugShot] section of gf2asi_settings.ini
 * overrides them.
 */
struct MugShotTuning
{
	// Raise the head shot render target above its stock 128x128.
	bool bEnable = true;

	// Square size of the head shot texture, in pixels. Clamped to [128, 4096],
	// where 128 is the stock size. The texture is A8R8G8B8 with one mip, so it
	// costs size * size * 4 bytes - 1 MB at 512, 4 MB at 1024.
	uint32_t Resolution = 512;
};

class Settings : public SH::Singleton<Settings>
{
public:

	void Init();

	// Resolved path to gf2asi_settings.ini, holding everything that is not a key binding.
	const std::wstring& GetSettingsFilePath() const { return SettingsFilePath; }

	// Resolved path to gf2asi_keybinds.ini, so KeybindManager can load and persist the
	// [Keybinds] section against the same file this class reads flyup/flydown from.
	const std::wstring& GetKeybindsFilePath() const { return KeybindsFilePath; }

	int GetFlyModeUpInput() const;
	int GetFlyModeDownInput() const;
	bool WantsPreOrderBonus() const { return bWantsPreOrderBonus; }
	bool ApplyCPUFix() const { return bApplyCPUFix; }
	bool ApplyBrightnessFix() const { return bApplyBrightnessFix; }
	bool ApplyVSyncFix() const { return bApplyVSyncFix; }
	float GetGammaContrast() const { return GammaContrast; }
	const EdgeAATuning& GetEdgeAATuning() const { return EdgeAA; }
	const MugShotTuning& GetMugShotTuning() const { return MugShot; }

	/**
	 * Persist the current photo mode camera settings to gf2asi_settings.ini.
	 * Creates the file (and section) if it doesn't exist yet.
	 */
	void SaveCameraSettings() const;

private:

	/**
	 * Seed the split config files from a pre-split gf2asi.ini, so an existing install
	 * keeps its settings and key bindings across the rename. Only fills in files that
	 * are not there yet, and leaves the legacy file on disk untouched.
	 */
	void MigrateLegacyConfigFile() const;

	// Resolved paths to the two config files, cached by Init() so saves target the
	// same files the load read from
	std::wstring SettingsFilePath;
	std::wstring KeybindsFilePath;

	// Virtual Key to get up in fly mode, read from [Keybinds] flyup in the keybinds file.
	// Held rather than pressed, which is why it is not a KeybindManager ShortcutAction.
	int FlyModeUpInput = VK_PRIOR;

	// Virtual Key to get down in fly mode, read from [Keybinds] flydown alongside the above
	int FlyModeDownInput = VK_NEXT;

	// Whether or not the Player wants Pre-order bonus unlocked
	bool bWantsPreOrderBonus = true;

	// Whether or not we should try and limit CPU count
	bool bApplyCPUFix = true;

	// Whether the brightness/gamma option should work in windowed mode and
	// stop clobbering desktop calibration in fullscreen.
	// Default off until it has been tested enough for release.
	bool bApplyBrightnessFix = false;

	// Whether the VSync option should present every refresh instead of every
	// second one. Stock, enabling VSync locks the game to 30 FPS whatever
	// resolution and refresh rate are selected. See SH_VSyncFix.
	bool bApplyVSyncFix = true;

	// Value for the first slot of the gamma triple the game hands to
	// Displ_SetGamma - the exponent the ramp is raised to. Valid range is
	// [0.8, 1.2]; the PC build runs at 0.8, the console versions default to
	// 1.0. Higher is darker and punchier, lower is brighter and flatter.
	// See SH_GammaFix.
	float GammaContrast = 0.8f;

	// Tuning for the forced EdgeAA post-process, see SH_EdgeAA
	EdgeAATuning EdgeAA;

	// Tuning for the mobface head shot capture, see SH_MugShotFix
	MugShotTuning MugShot;
};
