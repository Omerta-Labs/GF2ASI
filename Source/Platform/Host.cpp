#include "Platform/Host.h"

namespace EARS::Host
{
	namespace
	{
		// Zero-initialised: with no sinks installed, nothing owns the input,
		// which is how the game behaves with no overlay present.
		Sinks g_Sinks;
	}

	void InstallSinks(const Sinks& InSinks)
	{
		g_Sinks = InSinks;
	}

	bool OwnsCursor()
	{
		return g_Sinks.OwnsCursor != nullptr && g_Sinks.OwnsCursor();
	}

	bool OwnsKeyboard()
	{
		return g_Sinks.OwnsKeyboard != nullptr && g_Sinks.OwnsKeyboard();
	}
}
