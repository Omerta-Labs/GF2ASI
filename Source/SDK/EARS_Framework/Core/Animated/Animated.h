#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Entity/Entity.h"

// forward declares
namespace EA
{
	namespace CCT
	{
		class AnimView;
	}
}

namespace EARS
{
	namespace Framework
	{
		class Animated : public Entity
		{
		public:

			// getters
			EA::CCT::AnimView* GetAnimViewInfo() const { return m_AnimViewInfo; }

		private:

			char m_Animated_Padding[0x9C];
			void* m_CctInfo = nullptr;		// 0x20C
			RWS::CEventId m_MsgSetAnimTrigger;
			EA::CCT::AnimView* m_AnimViewInfo = nullptr;
		};
	} // Modules
} // EARS
