#pragma once

// SDK
#include "SDK/EARS_Framework/Game_Framework/Core/Base/Base.h"

namespace EARS
{
	namespace Framework
	{
		class Entity : public EARS::Framework::Base
		{
		public:

		private:

			char m_EntityPadding_0[0xE0];
			void* m_MetaModelInstance = nullptr; // 0x130 (EARS::Framework::MetaModelInstance)
			char m_EntityPadding_1[0x34];
			void* m_InterpolationMtx = nullptr; // 0x168 Interpolation Matrix? (EARS::Framework::Modules::imatrix44_t)
			void* m_AudOcclusionCB = nullptr; // 0x16C
			// Then entity finishes with one byte? TODO
		};
	} // Modules
} // EARS
