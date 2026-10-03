#include "Platform/Host.h"

namespace EARS::Host
{
	namespace
	{
		// Written once during start-up; see Diagnostics.cpp for the threading
		// caveat.
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
