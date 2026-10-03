#pragma once

// SDK Framework
#include "framework/core/base/base.h"

// C++
#include <stdint.h>

namespace EARS
{
	/**
	 * Flags stored in DirectionalShadowParamData::m_Flags.
	 * Bit 31 is reserved by the engine as an "author never touched this" marker.
	 */
	enum class DirectionalShadowFlags : uint32_t
	{
		DirectionalShadowFlags_REF = 0xFFFFFFFF,

		// Use m_Direction / m_Range instead of deriving them from the shadow light
		DIRECTIONAL_SHADOW_USE_AUTHORED_DIRECTION = 0x1,

		// Skip the character vertical-range fit (ShadowHelper char shadow vert range)
		DIRECTIONAL_SHADOW_SKIP_CHARACTER_VERTICAL_RANGE = 0x4,

		// Set by Reset(), cleared by Validate() once an attribute packet has been parsed
		DIRECTIONAL_SHADOW_NEVER_AUTHORED = 0x80000000,
	};

	/**
	 * Parameters describing one directional shadow caster.
	 *
	 * The engine hands these around by pointer - ShadowLightCameraManager stores one
	 * pointer per shadow caster slot and reads the values while building the shadow
	 * camera. Must stay 16 byte aligned; the engine asserts on that in its constructor.
	 */
	struct alignas(16) DirectionalShadowParamData
	{
		// Light direction, w is always forced to 0. Authored as the 'at' row of a matrix
		float m_Direction[4] = {};

		// Orthographic half extent of the shadow camera. farZ = nearZ + (2 * m_Range)
		// Overrides ShadowHelper::GetDirLightShadowDistance() when the authored flag is set
		float m_Range = 7.0f;

		// Height of the shadow camera above the centre of interest
		// Overrides ShadowHelper::GetDirLightShadowHeight()
		float m_Height = 80.0f;

		// See DirectionalShadowFlags
		uint32_t m_Flags = static_cast<uint32_t>(DirectionalShadowFlags::DIRECTIONAL_SHADOW_NEVER_AUTHORED);

	private:

		uint8_t m_Padding_1C[0x4] = {};

	public:

		/** Restore the shipped defaults and mark the data as never authored */
		void Reset();

		/** Clear the never-authored bit - the engine calls this after parsing a packet */
		void Validate();

		/** True while the never-authored bit is still set */
		bool IsInvalid() const;

		/** True when m_Direction and m_Range should be used instead of the light's own */
		bool UsesAuthoredDirection() const;
	};
	static_assert(sizeof(DirectionalShadowParamData) == 0x20, "EARS::DirectionalShadowParamData must equal 0x20");

	/**
	 * RWS attribute handler that owns a single DirectionalShadowParamData.
	 *
	 * The wrapper itself carries no state beyond the embedded data - it exists so the
	 * parameters can be authored in a packet and handed to the shadow system.
	 *
	 * NB: the embedded data sits at 0x50 on PC and 0x60 on Xbox 360, because
	 * EARS::Framework::Base is a different size on the two platforms.
	 */
	class DirectionalShadowParams : public EARS::Framework::Base
	{
	public:

		const DirectionalShadowParamData& GetData() const { return m_Data; }
		DirectionalShadowParamData& GetData() { return m_Data; }

	private:

		DirectionalShadowParamData m_Data;
	};
	static_assert(sizeof(DirectionalShadowParams) == 0x70, "EARS::DirectionalShadowParams must equal 0x70");
} // EARS
