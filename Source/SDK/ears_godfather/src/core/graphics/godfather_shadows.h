#pragma once

// SDK Framework
#include "SDK/ears_framework/src/framework/modules/lights/directionalshadowparams.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		// forward declares
		// NB: only ever handled as an opaque pointer here, the SDK has no EARSLight class yet
		class EARSLight;

		/**
		 * Owns the shadow casters Godfather II feeds to the framework shadow system.
		 *
		 * Every frame SetupShadowCastersCallback() fills three caster slots:
		 *   slot 0 and 1 - the two most interesting shadowed lights near the player
		 *   slot 2       - the vertical shadow light below
		 *
		 * The vertical shadow light is not the sun. It is a light created at startup whose
		 * matrix is rebuilt each frame pointing straight down (0, -1, 0) at the player, which
		 * is why world shadows in this game are top down rather than sun aligned. The sun
		 * direction that the sky shaders use lives in GlobalSkyData and does not drive this.
		 */
		class GodfatherShadowManager
		{
		public:

			/** The live instance, or nullptr before the graphics system has started */
			static GodfatherShadowManager* GetInstance();

			/** The straight down light used for slot 2. Opaque - the SDK has no EARSLight yet */
			EARSLight* GetVerticalShadowLight() const { return m_pVerticalShadowLight; }

			/** Parameters slot 2 is built with. Owned by the manager, never null in practice */
			DirectionalShadowParamData* GetShadowParams() const { return m_pShadowParams; }

			/**
			 * Restores the range and height the manager sets up at construction.
			 * Range is half of the first cascade distance, which is a live value, so this
			 * has to be read back rather than hard coded.
			 */
			void RestoreDefaultShadowParams();

		private:

			void* m_pVTable;                                    // 0x00
			uint32_t m_Unknown_04;                              // 0x04
			uint32_t m_Unknown_08;                              // 0x08 - overrides the shadow casting light when set
			EARSLight* m_pVerticalShadowLight;                  // 0x0C
			DirectionalShadowParamData* m_pShadowParams;        // 0x10

			// The tail of this class has not been mapped yet
		};
	} // Modules
} // EARS
