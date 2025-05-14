#include "SDK/EARS_RT_CCT/include/ChrCntrl_Character.h"

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
	} // CCT
} // EA
