#pragma once

//=============================================================================
// Logging and debug drawing available to the SDK.
//
// The SDK declares these; whatever is hosting it supplies the implementations
// through InstallSinks. With nothing installed every call is a no-op, so SDK
// code can log and draw unconditionally without caring whether a host is
// present.
//=============================================================================

#include "ears_common/rwtypes.h"

#include <cstdarg>
#include <cstdint>

namespace EARS::Diag
{
	/**
	 * Any member left null is not called, so a partial install is valid -- a
	 * host with no renderer can supply Printf alone.
	 */
	struct Sinks
	{
		void (*Printf)(const char* Format, va_list Args) = nullptr;

		void (*DrawLine)(const RwV3d& Start, const RwV3d& End, uint32_t Colour) = nullptr;
		void (*DrawSphere)(const RwV3d& Centre, float Radius, uint32_t Colour) = nullptr;
		void (*DrawBox)(const RwV3d& Min, const RwV3d& Max, uint32_t Colour) = nullptr;
		void (*DrawText3D)(const RwV3d& Position, const char* Text, uint32_t Colour) = nullptr;
	};

	/**
	 * Replaces the whole table. Call during start-up, before any SDK system
	 * runs; passing a default-constructed Sinks silences everything again.
	 */
	void InstallSinks(const Sinks& InSinks);

	/**
	 * Whether anything is listening. Worth testing before building a debug
	 * string that would otherwise be formatted and thrown away.
	 */
	bool HasSinks();

	/** Opaque white, 0xAARRGGBB. */
	inline constexpr uint32_t DEFAULT_COLOUR = 0xFFFFFFFFu;

	void Printf(const char* Format, ...);

	void DrawLine(const RwV3d& Start, const RwV3d& End,
	              uint32_t Colour = DEFAULT_COLOUR);
	void DrawSphere(const RwV3d& Centre, float Radius,
	                uint32_t Colour = DEFAULT_COLOUR);
	void DrawBox(const RwV3d& Min, const RwV3d& Max,
	             uint32_t Colour = DEFAULT_COLOUR);
	void DrawText3D(const RwV3d& Position, const char* Text,
	                uint32_t Colour = DEFAULT_COLOUR);
}
