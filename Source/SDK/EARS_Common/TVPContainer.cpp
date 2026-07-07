#include "SDK/EARS_Common/TVPContainer.h"

using namespace EARS::Common;

EA::TagValuePair TVPContainer::s_nullTVP;

EA::TagValuePair* TVPContainer::CopyTVP(const EA::TagValuePair& InTVP, EA::Allocator::IAllocator* InAllocator)
{
	EA::TagValuePair* pRet = nullptr;

	if (InTVP.m_Tag != 0 || InTVP.m_Next != nullptr)
	{
		// count the entries in the source chain
		int32_t NumPairs = 0;
		for (const EA::TagValuePair* pPair = &InTVP; pPair; pPair = pPair->m_Next)
		{
			NumPairs++;
		}

		pRet = static_cast<EA::TagValuePair*>(InAllocator->Alloc(NumPairs * sizeof(EA::TagValuePair), EA::TagValuePair(0u, 0)));

		// copy the chain into the contiguous block, relinking as we go
		EA::TagValuePair* pDst = pRet;
		const EA::TagValuePair* pSrc = &InTVP;
		while (true)
		{
			pDst->m_Tag = pSrc->m_Tag;
			pDst->m_Value = pSrc->m_Value;
			pDst->m_Next = pSrc->m_Next;

			pSrc = pSrc->m_Next;
			if (!pSrc)
			{
				pDst->m_Next = nullptr;
				break;
			}

			pDst->m_Next = pDst + 1;
			pDst++;
		}
	}

	return pRet;
}
