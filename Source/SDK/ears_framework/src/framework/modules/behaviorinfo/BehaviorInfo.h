#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Framework
	{
		class BehaviorInfo
		{
		public:

			virtual ~BehaviorInfo() { /* implemented in engine */ }

		private:

			uint32_t m_EnabledBehaviours = 0;
			bool m_bDirty = false;
		};
	}
}
