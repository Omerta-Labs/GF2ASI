#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Godfather
	{
		/**
		 * One keyframe of sky state on the time of day curve.
		 *
		 * The sun is authored as a transform, not as a direction. HandleAttributes reads a
		 * matrix into m_Transform, builds m_Orientation from it, then CalcSunDirection()
		 * rotates the +Z axis by that quaternion and caches the result into the SUN_DIR
		 * float params. Writing the SUN_DIR params directly will be overwritten the next
		 * time the orientation is evaluated or interpolated.
		 */
		class SetSkyMessage
		{
		public:

			static constexpr uint32_t NUM_SKY_COLOR_PARAMS = 4;
			static constexpr uint32_t NUM_SKY_FLOAT_PARAMS = 21;

			enum class SkyColorParam : uint32_t
			{
				SkyColorParam_REF = 0xFFFFFFFF,

				DIFFUSION_COLOR = 0x0,
				DIFFUSION_COLOR_LOW = 0x1,
				SKY_COLOR = 0x2,
				SKY_COLOR_LOW = 0x3,

				SkyColorParam_MAX_VALUE = 0x4,
			};

			/**
			 * Names come from the debug build, which kept the DEFAULT_ constants these are
			 * initialised from. The start and end distance pairs are split between the sky
			 * dome and the cloud sheet, confirmed by what SkyTODSequence::PostEvaluate
			 * hands to SetSkyDomeFogParams and SetSkyCloudsFogParams.
			 */
			enum class SkyFloatParam : uint32_t
			{
				SkyFloatParam_REF = 0xFFFFFFFF,

				SUN_POWER = 0x0,
				SUN_DIR_X = 0x1,
				SUN_DIR_Y = 0x2,
				SUN_DIR_Z = 0x3,

				DIFFUSION_PEAK = 0x4,
				DIFFUSION_EXPONENT = 0x5,
				DIFFUSION_RAMP_UP = 0x6,
				DIFFUSION_RAMP_DOWN = 0x7,

				CLOUD_OPAQUE_DENSITY = 0x8,
				SKY_DIFFUSION_EXP = 0x9,
				SKY_DIFFUSION_EXP_LOW = 0xA,
				HAZE_BAND_ANGLE = 0xB,

				SKY_START_DIST = 0xC,
				SKY_END_DIST = 0xD,
				CLOUD_START_DIST = 0xE,
				CLOUD_END_DIST = 0xF,

				SKY_FOG_START_DIST = 0x10,
				SKY_FOG_END_DIST = 0x11,
				CLOUD_FOG_START_DIST = 0x12,
				CLOUD_FOG_END_DIST = 0x13,

				// Defaults to 500.0, no consumer found yet
				UNKNOWN_14 = 0x14,

				SkyFloatParam_MAX_VALUE = 0x15,
			};

			float GetSkyParamF(const SkyFloatParam InParam) const;
			void SetSkyParamF(const SkyFloatParam InParam, const float InValue);

			const float* GetSkyParamC(const SkyColorParam InParam) const;
			void SetSkyParamC(const SkyColorParam InParam, const float InColor[4]);

			// Authored sun orientation. The identity quaternion points the sun along +Z
			const float* GetOrientation() const { return m_Orientation; }
			void SetOrientation(const float InQuaternion[4]);

			/**
			 * Rebuilds SUN_DIR_X/Y/Z by rotating (0, 0, 1) with m_Orientation.
			 * Normalises the quaternion first if it has drifted off unit length.
			 */
			void CalcSunDirection();

			/** Writes the values the game ships with */
			void SetDefaultSkyParams();

		private:

			// EARS::Modules::TOD::TODMessage, 0x4C on both PC and Xbox 360.
			// Held as a blob because the SDK has no TODMessage class yet
			uint8_t m_TODMessage[0x4C];                             // 0x00
			uint8_t m_Padding_4C[0x4];                              // 0x4C

			float m_Transform[4][4];                                // 0x50
			float m_Orientation[4];                                 // 0x90
			float m_ColorParams[NUM_SKY_COLOR_PARAMS][4];           // 0xA0
			float m_FloatParams[NUM_SKY_FLOAT_PARAMS];              // 0xE0

			uint8_t m_Unknown_134[0x8];                             // 0x134 - VFX handle region

			// 1 means the celestial VFX is positioned on the authoritative player
			int32_t m_VFXPositionMode;                              // 0x13C
		};

		// Self check on the padding above, the engine class continues past 0x140
		static_assert(sizeof(SetSkyMessage) == 0x140, "EARS::Godfather::SetSkyMessage layout drifted");
	} // Godfather
} // EARS
