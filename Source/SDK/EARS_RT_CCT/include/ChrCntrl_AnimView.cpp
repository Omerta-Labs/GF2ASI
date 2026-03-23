#include "ChrCntrl_AnimView.h"

namespace EA
{
	namespace CCT
	{
		/* NOT COMPLETE
		int AnimView::ChangeState(EA::CCT::AnimView& InAnimView, const int32_t InStateID)
		{
			if (InStateID == -1)
			{
				m_StateID = -1;
				m_RootStateID = -1;
				m_AnimTree = nullptr;

				return 0;
			}

			const ChrCntl_AnimNode_s* AnimNode = Character_GetStateNode(*InAnimView.m_Character, InStateID);
			if (AnimNode == nullptr)
			{
				// more like assert
				return 0;
			}

			ClearFlags(3);
			SetFlags(0x20020);
			m_StateID = InStateID;
			m_RootStateID = InStateID;
			m_AnimTree = AnimNode;
			m_FrameNum = 0.0f;
			m_PrevFrameNum = 0.0f;
			InAnimView.m_ActiveState = this;
			// ChrCntl_AnimTree_NodeEntered
			// ChrCntl_AnimTree_EvalVariables
			m_PrevNumFrames = m_NumFrames;
			// EA::CCT::Internal::AnimState::_UpdateFrameCount(this, pAnimView);
			const char* StateName = Character_GetStateName(*InAnimView.m_Character, InStateID);
			if(StateName)
			return 0;
		} */
	} // CCT
} // EA
