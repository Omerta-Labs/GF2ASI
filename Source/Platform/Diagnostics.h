#pragma once

//=============================================================================
// Logging and debug drawing for the SDK, without depending on the layer that
// provides them.
//
// The SDK sits at the bottom: GF2Mod depends on it, not the other way round. It
// still needs to print and to draw, so it declares the API here and GF2Mod
// installs the implementations at startup. Until then every call is a no-op, so
// GF2SDK links and runs on its own.
//
// Before this, three SDK files called tConsole::fPrintf directly -- SimManager,
// StreamManager and ShaderManager -- which made the bottom layer include from
// the layer above it.
//=============================================================================

#include "ears_common/rwtypes.h"

#include <cstdarg>
#include <cstdint>

namespace EARS::Diag
{
	/**
	 * Implementations supplied by the modding layer. Any member left null is
	 * simply not called, so a partial install is fine -- a tool that wants
	 * logging but has no renderer can fill in Printf alone.
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
	 * Called once from GF2Hook during start-up, before any SDK system runs.
	 * Replaces the whole table; pass a default-constructed Sinks to silence
	 * everything again.
	 */
	void InstallSinks(const Sinks& InSinks);

	/** Whether anything is listening. Worth checking before building an
	 *  expensive debug string that would be thrown away. */
	bool HasSinks();

	/** Default colour for the draw helpers: opaque white, 0xAARRGGBB. */
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
