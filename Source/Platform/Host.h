#pragma once

//=============================================================================
// Questions the SDK needs to ask of whatever is hosting it.
//
// The only one so far is input ownership. marketingdebug.cpp drives a free
// camera from the mouse and keyboard, and has to stand down while the menu
// overlay has them -- otherwise moving the mouse over the menu flies the
// camera. It asked ImGuiManager directly, which made an SDK file include from
// the modding layer.
//
// Same shape as Platform/Diagnostics.h: the SDK declares, GF2Mod installs, and
// the default answer is "nobody owns the input", which is how the game behaves
// with no overlay present.
//
// This is deliberately not a general "call the host" escape hatch. Anything
// added here should be a question about the environment the SDK is running in,
// not a way to reach a particular mod feature.
//=============================================================================

namespace EARS::Host
{
	struct Sinks
	{
		/** An overlay has the mouse, so game cameras should ignore it. */
		bool (*OwnsCursor)() = nullptr;

		/** An overlay has the keyboard, so game input should ignore it. */
		bool (*OwnsKeyboard)() = nullptr;
	};

	void InstallSinks(const Sinks& InSinks);

	bool OwnsCursor();
	bool OwnsKeyboard();
}
