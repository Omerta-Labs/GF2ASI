#pragma once

// SDK
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Framework/Core/Animated/Animated.h"

namespace EARS
{
	namespace Modules
	{
		// TODO: Move elsewhere when it has a home
		class PresetInstance
		{

		};

		/**
		 * An object in the world to describe an animated
		 * model using the Parted Model Manager system
		 * to visualise the mesh
		 */
		class PartedAnimated : public EARS::Framework::Animated
		{
		public:

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

			char m_PartedAnimated_Padding_0[0x98];
			String m_PartsPresetName; // 0x2B0 // (EARS::Modules::PartedAnimated)
			String m_PartsThemeName; // 0x2C0 // (EARS::Modules::PartedAnimated)
			String m_PartsSwapSetName; // 0x2D0 // (EARS::Modules::PartedAnimated)
			String m_PartsAssemblyName; // 0x2E0 // (EARS::Modules::PartedAnimated)
			void* m_PartsPresetInstance = nullptr; // 0x2F0 - 0x2F4 // (EARS::Modules::PartedAnimated)
		};
	}
}
