#pragma once

// CCT
#include "ChrCntrl_Transform.h"

// CPP
#include <stdint.h>

namespace EA
{
	namespace CCT
	{
		namespace Internal
		{
			class AnimState
			{
			public:

				virtual ~AnimState() { /* filled in by game code */ }

			protected:

				char pad_0000[16]; //0x0000 (flags in here somewhere, maybe 0x10)
				uint32_t m_StateID = 0; //0x0014
				uint32_t m_RootStateID = 0; //0x0018
				float m_FrameNum = 0.0f; //0x001C
				float m_PrevFrameNum = 0.0f; //0x0020
				float m_NumFrames = 0.0f; //0x0024
				float m_PrevNumFrames = 0.0f; //0x0028
				int32_t m_BlendOutMode = 0; //0x002C
				char pad_0030[16]; //0x0030
				EA::CCT::Transform m_CarryOver; // m_gmCarryOver
				void* m_AnimTree; //0x0060
				void* m_InstVars; //0x0064
				void* m_NodeToUpdate; //0x0068
				char m_AnimState_padding_1[4];

			};
		}
	} // CCT
} // EARS
