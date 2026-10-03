#pragma once

// SDK Framework
#include "SDK/ears_framework/src/framework/modules/lights/directionalshadowparams.h"

// C++
#include <stdint.h>

// forward declares
class RenderContext;

namespace EARS
{
	namespace Modules
	{
		// forward declares
		// NB: only ever handled as an opaque pointer here, the SDK has no EARSLight class yet
		class EARSLight;
	}

	/**
	 * Holds the shadow casters the renderer builds shadow cameras for.
	 *
	 * The game fills the slots through a setup callback that runs once per frame, then
	 * CalculateShadowCameraMatrix() reads the light and the parameters back out of each
	 * slot. A slot with no parameters falls back to the ShadowHelper defaults.
	 */
	class ShadowLightCameraManager
	{
	public:

		static constexpr uint32_t NUM_SHADOW_CASTERS = 3;

		/** The live instance, or nullptr before the rendering system has started */
		static ShadowLightCameraManager* GetInstance();

		/** Light used for a caster slot. Opaque - the SDK has no EARSLight yet */
		EARS::Modules::EARSLight* GetShadowLight(const uint32_t InSlot) const;
		void SetShadowLight(const uint32_t InSlot, EARS::Modules::EARSLight* InLight);

		/**
		 * Parameters used for a caster slot. The manager only stores the pointer, it does
		 * not own the data, so whatever is pointed at has to outlive the frame.
		 */
		DirectionalShadowParamData* GetShadowParams(const uint32_t InSlot) const;
		void SetShadowParams(const uint32_t InSlot, DirectionalShadowParamData* InParams);

		/** Clears every slot */
		void ClearShadowCasters();

		/** Parameters the manager constructs for itself, separate from the slots above */
		DirectionalShadowParamData& GetDefaultParams() { return m_DefaultParams; }

	private:

		void* m_pVTable;                                                // 0x00
		uint8_t m_Padding_04[0xC];                                      // 0x04

		DirectionalShadowParamData m_DefaultParams;                     // 0x10
		EARS::Modules::EARSLight* m_pShadowLights[NUM_SHADOW_CASTERS];  // 0x30
		DirectionalShadowParamData* m_pShadowParams[NUM_SHADOW_CASTERS];// 0x3C

		void (*m_pSetupCallback)(RenderContext*);                       // 0x48

		bool m_bUnknown_4C;                                             // 0x4C - true at construction
		bool m_bUnknown_4D;                                             // 0x4D - true at construction

		// The tail of this class has not been mapped yet
	};
} // EARS
