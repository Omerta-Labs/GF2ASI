#pragma once

// CCT
#include "ChrCntrl_Transform.h"

// CPP
#include <stdint.h>

// forward declares
struct ChrCntl_AnimNode_s;

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

				/** Add Flags to the AnimState instance */
				void SetFlags(int32_t InFlags);

				/** Remove Flags from the AnimState instance */
				void ClearFlags(int32_t InFlags);

				// getters
				uint32_t GetStateId() const { return m_StateID; }
				int32_t GetFlags() const { return m_Flags; }
				float GetFrameNum() const { return m_FrameNum; }
				float GetNumFrames() const { return m_NumFrames; }
				bool IsLooping() const { return m_bLooping; }

				/** True when the animation has finished playing.
				 *  Checks: NumFrames <= 0.0f OR anim-end flags (bits 0,1,25) are set.
				 *  PC pattern: sub_4AC260 + sub_572530 both use this exact test. */
				bool IsAnimDone() const
				{
					return (m_NumFrames <= 0.0f) || (m_Flags & 0x2000003) != 0;
				}

			protected:

				char pad_0000[12]; //0x0000
				int32_t m_Flags = 0; // 0x0010
				uint32_t m_StateID = 0; //0x0014
				uint32_t m_RootStateID = 0; //0x0018
				float m_FrameNum = 0.0f; //0x001C
				float m_PrevFrameNum = 0.0f; //0x0020
				float m_NumFrames = 0.0f; //0x0024
				float m_PrevNumFrames = 0.0f; //0x0028
				int32_t m_BlendOutMode = 0; //0x002C
				bool m_bLooping = false; //0x0030 — true when this anim is a looping anim (read by IsLoopingAnim)
				char pad_0031[15]; //0x0031
				EA::CCT::Transform m_CarryOver; // m_gmCarryOver
				const ChrCntl_AnimNode_s* m_AnimTree; //0x0060
				void* m_InstVars; //0x0064
				void* m_NodeToUpdate; //0x0068
				char m_AnimState_padding_1[4];

			};
		}
	} // CCT
} // EARS
