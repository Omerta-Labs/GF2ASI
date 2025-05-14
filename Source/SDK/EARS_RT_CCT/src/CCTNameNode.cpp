#include "SDK/EARS_RT_CCT/include/CCTNameNode.h"

namespace EA
{
	namespace CCT
	{
		CCTNameNode::CCTNameNode()
			: m_Name(nullptr)
			, m_Node(nullptr)
		{
			// otherwise empty
		}

		CCTNameNode::CCTNameNode(const char* InName, ChrCntl_AnimNode_s* InNode)
			: m_Name(InName)
			, m_Node(InNode)
		{
			// otherwise empty
		}

		CCTNameNode::~CCTNameNode()
		{
			m_Name = nullptr;
			m_Node = nullptr;
		}

		void CCTNameNode::Relocate(int32_t InOffset)
		{
			m_Node = (m_Node + InOffset);
			m_Name += InOffset;
		}
	} // CCT
} // EA
