#pragma once

// SDK Framework
#include "SDK/ears_framework/src/framework/core/base/Base.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Framework
	{
		/**
		 * Global gamma correction, authored as an attribute packet.
		 *
		 * Despite the broad name this class only carries gamma. Parsing a packet fills
		 * m_GammaCorrection and, when m_bApplyOnLoad is set, immediately marshals the four
		 * floats to the render thread as CMD_RenderingParamsSetGamma.
		 *
		 * Writing the values here does not change anything on its own - the render thread
		 * holds its own copy, so the command has to be issued for a change to be visible.
		 */
		class RenderingParams : public EARS::Framework::Base
		{
		public:

			/** Attribute class the packet parser matches on */
			static constexpr uint32_t CLASS_ID = 0xC449D5D2;

			/** The live instance, or nullptr before the rendering system has started */
			static RenderingParams* GetInstance();

			const float* GetGammaCorrection() const { return m_GammaCorrection; }

			/**
			 * Stores a new gamma ramp. Component 3 is never authored by the game, the
			 * attribute parser skips command 3, so it stays whatever it was.
			 */
			void SetGammaCorrection(const float InRed, const float InGreen, const float InBlue);

			bool ShouldApplyOnLoad() const { return m_bApplyOnLoad; }
			void SetApplyOnLoad(const bool bInApply) { m_bApplyOnLoad = bInApply; }

			/** Writes the values the game ships with */
			void RestoreDefaults();

		private:

			// Singleton<RenderingParams> sub object, its vtable sits at 0x50
			uint8_t m_Padding_50[0x10];                 // 0x50

			float m_GammaCorrection[4];                 // 0x60
			bool m_bApplyOnLoad;                        // 0x70
		};

		static_assert(sizeof(RenderingParams) == 0x74, "EARS::Framework::RenderingParams must equal 0x74");
	} // Framework
} // EARS
