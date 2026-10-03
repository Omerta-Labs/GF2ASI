#pragma once

// SDK
#include "SDK/ears_common/include/ears_common/String.h"
#include "SDK/ears_framework/src/framework/core/animated/Animated.h"

namespace EARS
{
	namespace Modules
	{
		class PresetInstance;

		/**
		 * An object in the world to describe an animated
		 * model using the Parted Model Manager system
		 * to visualise the mesh
		 */
		class PartedAnimated : public EARS::Framework::Animated
		{
		public:

			virtual ~PartedAnimated() = 0;

			/**
			 * Rebuild the PartedAnimated object using the Parts described within the class.
			 * (NB: PART OF ENGINE CODE)
			 */
			void ForceModelRebuild(EARS::Modules::PresetInstance* NewInstance, const char* NewPresetName);

			/**
			 * Try swap the selected model on the fly.
			 * (NB: NOT PART OF ENGINE CODE)
			 */
			void TrySwapModel(const char* AssemblyName, const char* PresetName);

		protected:

			String m_PartsPresetName; // 0x2B0
			String m_PartsThemeName; // 0x2C0
			String m_PartsSwapSetName; // 0x2D0
			String m_PartsAssemblyName; // 0x2E0
			EARS::Modules::PresetInstance* m_PartsPresetInstance = nullptr; // 0x2F0 - 0x2F4
			char m_PartedAnimated_Padding0[0x9C];
		};

		static_assert(sizeof(PartedAnimated) == 0x390, "EARS::Framework::PartedAnimated must equal 0x390");
	}
}
