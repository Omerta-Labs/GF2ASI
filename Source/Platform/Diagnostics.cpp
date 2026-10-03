#include "Platform/Diagnostics.h"

namespace EARS::Diag
{
	namespace
	{
		// Written once during start-up and read from the sim and presentation
		// threads thereafter, so the unsynchronised access is safe only as long
		// as nothing installs sinks late.
		Sinks g_Sinks;
	}

	void InstallSinks(const Sinks& InSinks)
	{
		g_Sinks = InSinks;
	}

	bool HasSinks()
	{
		return g_Sinks.Printf != nullptr
			|| g_Sinks.DrawLine != nullptr
			|| g_Sinks.DrawSphere != nullptr
			|| g_Sinks.DrawBox != nullptr
			|| g_Sinks.DrawText3D != nullptr;
	}

	void Printf(const char* Format, ...)
	{
		if (g_Sinks.Printf == nullptr || Format == nullptr)
		{
			return;
		}

		// The sink takes a va_list rather than being variadic itself, so the
		// formatting stays on this side of the layer boundary and the
		// implementation can forward straight to vsnprintf.
		va_list Args;
		va_start(Args, Format);
		g_Sinks.Printf(Format, Args);
		va_end(Args);
	}

	void DrawLine(const RwV3d& Start, const RwV3d& End, const uint32_t Colour)
	{
		if (g_Sinks.DrawLine != nullptr)
		{
			g_Sinks.DrawLine(Start, End, Colour);
		}
	}

	void DrawSphere(const RwV3d& Centre, const float Radius, const uint32_t Colour)
	{
		if (g_Sinks.DrawSphere != nullptr)
		{
			g_Sinks.DrawSphere(Centre, Radius, Colour);
		}
	}

	void DrawBox(const RwV3d& Min, const RwV3d& Max, const uint32_t Colour)
	{
		if (g_Sinks.DrawBox != nullptr)
		{
			g_Sinks.DrawBox(Min, Max, Colour);
		}
	}

	void DrawText3D(const RwV3d& Position, const char* Text, const uint32_t Colour)
	{
		if (g_Sinks.DrawText3D != nullptr && Text != nullptr)
		{
			g_Sinks.DrawText3D(Position, Text, Colour);
		}
	}
}
