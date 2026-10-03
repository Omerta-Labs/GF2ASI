#include "SDK/ears_rt_cct/include/ears_rt_cct/ChrCntrl_Character.h"

namespace EA
{
	namespace CCT
	{
		const char* Character_GetStateName(const ChrCntl_ChrInfo_s& InCharacter, const uint32_t ID)
		{
			if (const CCTNameNode* Node = InCharacter.m_NameNodeHash->Lookup(ID))
			{
				return Node->GetName();
			}

			return nullptr;
		}

		const ChrCntl_AnimNode_s* Character_GetStateNode(const ChrCntl_ChrInfo_s& InCharacter, const uint32_t ID)
		{
			if (const CCTNameNode* Node = InCharacter.m_NameNodeHash->Lookup(ID))
			{
				return Node->GetNode();
			}

			return nullptr;
		}

		uint32_t Character_CountStateNodes(const ChrCntl_ChrInfo_s& InCharacter)
		{
			return InCharacter.m_NameNodeHash->Size();
		}

		uint32_t Character_CountStateNames(const ChrCntl_ChrInfo_s& InCharacter)
		{
			return InCharacter.m_NameNodeHash->Size();
		}

		int32_t Character_EnumerateStateNodes(const ChrCntl_ChrInfo_s& InCharacter, TEnumerateNodesCB Callback, void* InOutData)
		{
			int Index = 0;
			auto NodeIt = InCharacter.m_NameNodeHash->CreateIterator();

			// begin loop
			while (!NodeIt.IsFinshed())
			{
				// get data from node
				const uint32_t StateID = NodeIt.GetKey();
				const ChrCntl_AnimNode_s& AnimNode = *NodeIt.GetObject()->GetNode();

				// run callback, if comes back true, exit loop
				if (Callback(StateID, AnimNode, InOutData))
				{
					return Index;
				}

				Index++;
			}

			return -1;
		}

		int32_t Character_EnumerateStateNames(const ChrCntl_ChrInfo_s& InCharacter, TEnumerateNamesCB Callback, void* InOutData)
		{
			int Index = 0;
			auto NodeIt = InCharacter.m_NameNodeHash->CreateIterator();

			// begin loop
			while (!NodeIt.IsFinshed())
			{
				// get data from node
				const uint32_t StateID = NodeIt.GetKey();
				const char* StateName = NodeIt.GetObject()->GetName();

				// run callback, if comes back true, exit loop
				if (Callback(StateID, StateName, InOutData))
				{
					return Index;
				}

				Index++;
				NodeIt++;
			}

			return -1;
		}
	} // CCT
} // EA
