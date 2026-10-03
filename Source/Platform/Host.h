#pragma once

//=============================================================================
// Questions the SDK asks of whatever is hosting it.
//
// Input ownership is the only one so far: a host that draws an overlay takes
// the mouse and keyboard, and game cameras must stand down while it does, or
// moving the pointer over the overlay also drives the camera.
//
// With no sinks installed nothing owns the input, which is how the game behaves
// with no overlay present.
//
// Keep this to questions about the environment the SDK runs in, not a general
// route from the SDK to a particular host feature.
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
