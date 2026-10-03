#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Godfather
	{
		/**
		 * Base for every global block of shader constants.
		 *
		 * Carries the time of day the block was last evaluated at, so a TOD sequence can
		 * tell whether the values it is about to write are already current.
		 */
		template<typename T>
		class GlobalShaderData
		{
		public:

			virtual ~GlobalShaderData() = default;

			// Dumps the block to the debug display
			virtual void PrintDebugDisplay() const {}

			// EARS::Modules::TOD::Time - opaque here, the SDK has no Time class yet
			const uint8_t* GetTime() const { return m_Time; }

		protected:

			uint8_t m_Time[0x1C] = {};
		};

		/**
		 * Character lighting constants (rim light, movie light and two specular lights).
		 *
		 * Written by the GFCharLightingData attribute handler and by the shader debug menu,
		 * read by the character shaders. The world space direction copies are recomputed
		 * from the object space ones every frame by UpdateWorldSpaceLightDirections().
		 */
		class GlobalCharLightingData : public GlobalShaderData<GlobalCharLightingData>
		{
		public:

			static constexpr uint32_t NUM_SPEC_LIGHTS = 2;

			/** The live instance, or nullptr before the graphics system has started */
			static GlobalCharLightingData* GetInstance();

			// Colour in xyz, intensity in w
			const float* GetRimLight() const { return m_RimLight; }
			const float* GetMovieLight() const { return m_MovieLight; }

			// Rim light shaping parameters
			const float* GetRimLightParams() const { return m_RimLightParams; }
			const float* GetRimLightDiffuseSpecParams() const { return m_RimLightDiffuseSpecParams; }

			// Object space light directions
			const float* GetRimLightDirection() const { return m_RimLightDirection; }
			const float* GetMovieLightDirection() const { return m_MovieLightDirection; }
			const float* GetSpecLightDirection(const uint32_t InIndex) const;

			// World space copies, recomputed from the object space directions each frame
			const float* GetWorldSpaceRimLightDirection() const { return m_WorldSpaceRimLightDirection; }
			const float* GetWorldSpaceMovieLightDirection() const { return m_WorldSpaceMovieLightDirection; }
			const float* GetWorldSpaceSpecLightDirection(const uint32_t InIndex) const;

			float GetSpecLightIntensity(const uint32_t InIndex) const;
			float GetSpecLightExponent(const uint32_t InIndex) const;

			void SetRimLight(const float InColorAndIntensity[4]);
			void SetMovieLight(const float InColorAndIntensity[4]);
			void SetRimLightParams(const float InParams[4]);
			void SetRimLightDiffuseSpecParams(const float InParams[4]);

			void SetRimLightDirection(const float InDirection[4]);
			void SetMovieLightDirection(const float InDirection[4]);
			void SetSpecLightDirection(const uint32_t InIndex, const float InDirection[4]);

			void SetSpecLightIntensity(const uint32_t InIndex, const float InIntensity);
			void SetSpecLightExponent(const uint32_t InIndex, const float InExponent);

			/** Forces the world space directions to be rebuilt on the next update */
			void SetWorldSpaceLightDirectionsComputed(const bool bInComputed);

			/** Writes the values the game ships with, as set by the constructor */
			void RestoreDefaults();

		private:

			float m_RimLight[4];                                        // 0x20
			float m_MovieLight[4];                                      // 0x30
			float m_RimLightParams[4];                                  // 0x40
			float m_RimLightDiffuseSpecParams[4];                       // 0x50
			float m_RimLightDirection[4];                               // 0x60
			float m_MovieLightDirection[4];                             // 0x70
			float m_WorldSpaceMovieLightDirection[4];                   // 0x80
			float m_WorldSpaceRimLightDirection[4];                     // 0x90
			float m_SpecLightDirection[NUM_SPEC_LIGHTS][4];             // 0xA0
			float m_WorldSpaceSpecLightDirection[NUM_SPEC_LIGHTS][4];   // 0xC0

			// [i][0] is intensity, [i][1] is exponent, [i][2] and [i][3] are unused
			float m_SpecLight[NUM_SPEC_LIGHTS][4];                      // 0xE0

			bool m_bWorldSpaceLightDirectionsComputed;                  // 0x100
		};

		static_assert(sizeof(GlobalCharLightingData) == 0x104, "EARS::Godfather::GlobalCharLightingData must equal 0x104");

		/**
		 * Sky and sun constants, consumed by the sky dome and cloud shaders.
		 *
		 * Everything here is overwritten by SkyTODSequence::PostEvaluate whenever the time
		 * of day is evaluated, so writes only stick until the next TOD update.
		 */
		class GlobalSkyData : public GlobalShaderData<GlobalSkyData>
		{
		public:

			static constexpr uint32_t NUM_SKY_SHADER_LAYERS = 4;

			/** The live instance, or nullptr before the graphics system has started */
			static GlobalSkyData* GetInstance();

			// Sun direction in xyz, sun power in w. The direction is derived by the sky TOD
			// message from its authored orientation quaternion, it is not authored directly
			const float* GetSunDirAndPower() const { return m_SunDirAndPower; }
			void SetSunDirAndPower(const float InX, const float InY, const float InZ, const float InPower);

			// Peak density, exponent, ramp up rate, ramp down rate
			const float* GetDiffusionControl() const { return m_DiffusionControl; }
			void SetDiffusionControl(const float InPeakDensity, const float InExponent, const float InRampUpRate, const float InRampDownRate);

			const float* GetDiffusionColor() const { return m_DiffusionColor; }
			const float* GetDiffusionColorLow() const { return m_DiffusionColorLow; }
			const float* GetSkyColor() const { return m_SkyColor; }
			const float* GetSkyColorLow() const { return m_SkyColorLow; }

			void SetDiffusionColor(const float InColor[4]);
			void SetDiffusionColorLow(const float InColor[4]);
			void SetSkyColor(const float InColor[4]);
			void SetSkyColorLow(const float InColor[4]);

			float GetSkyDiffusionExp() const { return m_SkyDiffusionExp; }
			float GetSkyDiffusionExpLow() const { return m_SkyDiffusionExpLow; }
			float GetHazeBandAngle() const { return m_HazeBandAngle; }
			float GetCloudOpaqueDensity() const { return m_CloudOpaqueDensity; }

			void SetSkyDiffusionExp(const float InExp) { m_SkyDiffusionExp = InExp; }
			void SetSkyDiffusionExpLow(const float InExp) { m_SkyDiffusionExpLow = InExp; }
			void SetHazeBandAngle(const float InAngle) { m_HazeBandAngle = InAngle; }
			void SetCloudOpaqueDensity(const float InDensity) { m_CloudOpaqueDensity = InDensity; }

			// Fog is stored as start, fog start, and a scale derived from the two ranges
			void SetSkyDomeFogParams(const float InDomeStart, const float InDomeEnd, const float InFogStart, const float InFogEnd);
			void SetSkyCloudsFogParams(const float InCloudStart, const float InCloudEnd, const float InFogStart, const float InFogEnd);

			// Per layer cloud sheet controls
			const float* GetCloudScrollParams(const uint32_t InLayer) const;
			const float* GetAlphaScrollParams(const uint32_t InLayer) const;
			const float* GetTintColor(const uint32_t InLayer) const;

			void SetCloudScrollParams(const uint32_t InLayer, const float InUScrollRate, const float InVScrollRate, const float InUScale, const float InVScale);
			void SetAlphaScrollParams(const uint32_t InLayer, const float InUScrollRate, const float InVScrollRate, const float InUScale, const float InVScale);
			void SetTintColor(const uint32_t InLayer, const float InColor[4]);

			uint32_t GetCloudTexture(const uint32_t InLayer) const;
			uint32_t GetAlphaTexture(const uint32_t InLayer) const;
			void SetCloudTexture(const uint32_t InLayer, const uint32_t InTexture);
			void SetAlphaTexture(const uint32_t InLayer, const uint32_t InTexture);

			/**
			 * Writes the values the game ships with, as set by the constructor.
			 * The next time of day evaluation will overwrite them again.
			 */
			void RestoreDefaults();

		private:

			// uScrollRate, vScrollRate, uScale, vScale
			float m_CloudScrollParams[NUM_SKY_SHADER_LAYERS][4];        // 0x20
			float m_AlphaScrollParams[NUM_SKY_SHADER_LAYERS][4];        // 0x60
			float m_TintColor[NUM_SKY_SHADER_LAYERS][4];                // 0xA0

			float m_SunDirAndPower[4];                                  // 0xE0
			float m_DiffusionControl[4];                                // 0xF0
			float m_DiffusionColor[4];                                  // 0x100
			float m_DiffusionColorLow[4];                               // 0x110
			float m_SkyColor[4];                                        // 0x120
			float m_SkyColorLow[4];                                     // 0x130

			// start, fog start, scale, unused
			float m_SkyDomeFogParams[4];                                // 0x140
			float m_SkyCloudsFogParams[4];                              // 0x150

			uint32_t m_CloudTexture[NUM_SKY_SHADER_LAYERS];             // 0x160
			uint32_t m_AlphaTexture[NUM_SKY_SHADER_LAYERS];             // 0x170

			float m_SkyDiffusionExp;                                    // 0x180
			float m_SkyDiffusionExpLow;                                 // 0x184
			float m_HazeBandAngle;                                      // 0x188
			float m_CloudOpaqueDensity;                                 // 0x18C
		};

		static_assert(sizeof(GlobalSkyData) == 0x190, "EARS::Godfather::GlobalSkyData must equal 0x190");
	} // Godfather
} // EARS
