#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Godfather
	{
		/**
		 * One keyframe of global lighting attenuation on the time of day curve.
		 *
		 * Only the two attenuation params are applied - the sequence pushes them into
		 * SetMainLightShadowAttenuation and SetAllLightShadowAttenuation when it evaluates.
		 * Skin and Rigid are parsed, defaulted, copied and interpolated like the others,
		 * but nothing in the shipped code reads them back.
		 */
		class LightingTODMessage
		{
		public:

			static constexpr uint32_t NUM_LIGHTING_TOD_PARAMS = 4;

			/** Attribute class the packet parser matches on */
			static constexpr uint32_t CLASS_ID = 0xDA70156A;

			/** Names taken from the debug build's ToString format string */
			enum class LightingTODParams : uint32_t
			{
				LightingTODParams_REF = 0xFFFFFFFF,

				MAIN_ATTEN = 0x0,
				ALL_ATTEN = 0x1,

				// Authored and interpolated, but no consumer found in the shipped code
				SKIN = 0x2,
				RIGID = 0x3,

				LightingTODParams_MAX_VALUE = 0x4,
			};

			float GetParam(const LightingTODParams InParam) const;
			void SetParam(const LightingTODParams InParam, const float InValue);

			/** Writes the values the game ships with */
			void SetDefaultParams();

		private:

			// EARS::Modules::TOD::TODMessage, 0x4C on both PC and Xbox 360.
			// Held as a blob because the SDK has no TODMessage class yet
			uint8_t m_TODMessage[0x4C];                             // 0x00

			float m_Params[NUM_LIGHTING_TOD_PARAMS];                // 0x4C
		};
		static_assert(sizeof(LightingTODMessage) == 0x5C, "EARS::Godfather::LightingTODMessage must equal 0x5C");
	} // Godfather
} // EARS
