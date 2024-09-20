#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Entity/Entity.h"

namespace EARS
{
	namespace Framework
	{
		class Animated : public Entity
		{
		public:

		private:

			char m_Animated_Padding[0x98];
			void* m_CctInfo = nullptr;		// 0x20C
			RWS::CEventId m_MsgSetAnimTrigger;
			void* m_AnimViewInfo = nullptr;
		};
	} // Modules
} // EARS
