#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	/**
	 * Static tuning values shared by the whole shadow pipeline.
	 *
	 * These are plain globals in the engine rather than members, and several of them are
	 * live values rather than constants - the cascade distances are eased towards a target
	 * set every frame, so read them, do not assume the shipped numbers.
	 */
	class ShadowHelper
	{
	public:

		static constexpr uint32_t NUM_CASCADED_SHADOW_MAPS = 3;

		/** Cascade split distances, in world units. Eased towards a target set every frame */
		static float GetDirLightShadowDistance(const uint32_t InCascade);
		static void SetDirLightShadowDistance(const uint32_t InCascade, const float InDistance);

		/** Default height of a directional shadow camera above its centre of interest */
		static float GetDirLightShadowHeight();
		static void SetDirLightShadowHeight(const float InHeight);

		/** Vertical extent fitted around a character when it owns the shadow camera */
		static float GetCharShadowVertRangeMin();
		static float GetCharShadowVertRangeMax();
		static void SetCharShadowVertRange(const float InMin, const float InMax);

	private:

		// float[3] - EARS::ShadowHelper::kDirLightShadowDistances
		static constexpr uintptr_t ADDR_DIR_LIGHT_SHADOW_DISTANCES = 0x11280F0;

		// float - EARS::ShadowHelper::kDirLightShadowHeight
		static constexpr uintptr_t ADDR_DIR_LIGHT_SHADOW_HEIGHT = 0x110AC8C;

		// float - EARS::ShadowHelper::kCharShadowVertRangeMin
		static constexpr uintptr_t ADDR_CHAR_SHADOW_VERT_RANGE_MIN = 0x110AC90;

		// float - EARS::ShadowHelper::kCharShadowVertRangeMax
		static constexpr uintptr_t ADDR_CHAR_SHADOW_VERT_RANGE_MAX = 0x110AC94;
	};
} // EARS
