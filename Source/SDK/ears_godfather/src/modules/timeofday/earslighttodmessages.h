#pragma once

// SDK Common
#include "SDK/ears_common/include/ears_common/guid.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		namespace TOD
		{
			/**
			 * One keyframe of state for a single named light on the time of day curve.
			 *
			 * The message does not own a light. It references one by GUID, caches a safe
			 * pointer to it, and on evaluation pushes its parameters into that light through
			 * UpdateEARSLightParams().
			 *
			 * Shadow colour and density are packed together before they reach the light:
			 * the three shadow colour channels and SHADOW_DENSITY are each scaled by 255 and
			 * assembled into an RGBA byte quad.
			 */
			class EARSLightMessage
			{
			public:

				static constexpr uint32_t NUM_EARS_LIGHT_PARAMS = 10;

				/**
				 * Named from what UpdateEARSLightParams() does with each one. Params 7 and 9
				 * are interpolated with the rest but never applied to the light, so their
				 * meaning is unconfirmed - their defaults have the shape of depth biases.
				 */
				enum class EARSLightParam : uint32_t
				{
					EARSLightParam_REF = 0xFFFFFFFF,

					CONE_ANGLE = 0x0,
					RADIUS = 0x1,
					INTENSITY = 0x2,

					// Authored in degrees, applied as cosf(value * PI / 180)
					FULLBRIGHT_ANGLE = 0x3,
					FULLBRIGHT_RADIUS = 0x4,

					DIFFUSE_AMBIENT = 0x5,
					SPEC_SCALE = 0x6,

					UNKNOWN_7 = 0x7,

					// Becomes the alpha of the packed shadow colour
					SHADOW_DENSITY = 0x8,

					UNKNOWN_9 = 0x9,

					EARSLightParam_MAX_VALUE = 0xA,
				};

				float GetEARSLightParam(const EARSLightParam InParam) const;
				void SetEARSLightParam(const EARSLightParam InParam, const float InValue);

				const float* GetColor() const { return m_Color; }
				void SetColor(const float InColor[4]);

				// Only xyz are used, the density comes from the SHADOW_DENSITY param
				const float* GetShadowColor() const { return m_ShadowColor; }
				void SetShadowColor(const float InColor[4]);

				const EARS::Common::guid128_t& GetLightGUID() const { return m_LightGUID; }

				const float* GetTransform() const { return &m_Transform[0][0]; }

				bool UseAuthoredTransform() const { return m_bUseAuthoredTransform; }
				void SetUseAuthoredTransform(const bool bInUse) { m_bUseAuthoredTransform = bInUse; }

				/** Writes the values the game ships with */
				void SetDefaultParams();

			private:

				// EARS::Modules::TOD::TODMessage, 0x4C on both PC and Xbox 360.
				// Held as a blob because the SDK has no TODMessage class yet
				uint8_t m_TODMessage[0x4C];                     // 0x00

				float m_Params[NUM_EARS_LIGHT_PARAMS];          // 0x4C
				uint8_t m_Padding_74[0xC];                      // 0x74

				float m_Color[4];                               // 0x80
				float m_ShadowColor[4];                         // 0x90

				EARS::Common::guid128_t m_LightGUID;            // 0xA0

				// SafePtr<EARS::Modules::EARSLight>, blob because the SDK has no EARSLight
				uint8_t m_Light[0x10];                          // 0xB0

				float m_Transform[4][4];                        // 0xC0

				// QuatTransS, the interpolation friendly form of m_Transform
				uint8_t m_QuatTrans[0x20];                      // 0x100

				bool m_bUnknown_120;                            // 0x120
				bool m_bUseAuthoredTransform;                   // 0x121
			};

			// Self check on the padding above, the engine class may continue past 0x124
			static_assert(sizeof(EARSLightMessage) == 0x124, "EARS::Modules::TOD::EARSLightMessage layout drifted");
		} // TOD
	} // Modules
} // EARS
