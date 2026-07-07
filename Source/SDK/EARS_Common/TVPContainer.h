#pragma once

// SDK
#include "SDK/EARS_Common/IAllocator.h"

namespace EARS::Common
{
	/**
	 * Holds the allocator a container should allocate from, together with a
	 * deep copy of the allocation tag chain (EA::TagValuePair) it was given.
	 */
	struct TVPContainer
	{
	public:

		TVPContainer(const EA::TagValuePair& InTVP, EA::Allocator::IAllocator* InAllocator)
		{
			m_pTVP = CopyTVP(InTVP, InAllocator);
			m_pAllocator = InAllocator;
			m_pAllocator->AddRef();
		}

		TVPContainer(const TVPContainer& InOther)
		{
			m_pTVP = InOther.m_pTVP ? CopyTVP(*InOther.m_pTVP, InOther.m_pAllocator) : nullptr;
			m_pAllocator = InOther.m_pAllocator;
			m_pAllocator->AddRef();
		}

		~TVPContainer()
		{
			if (m_pTVP)
			{
				m_pAllocator->Free(m_pTVP, 0);
			}

			m_pAllocator->Release();
		}

		// Deep-copies a tag chain into one contiguous block allocated from the
		// given allocator. Returns null for an empty (default) tag.
		static EA::TagValuePair* CopyTVP(const EA::TagValuePair& InTVP, EA::Allocator::IAllocator* InAllocator);

		const EA::TagValuePair& Get() const { return m_pTVP ? *m_pTVP : s_nullTVP; }
		EA::Allocator::IAllocator& GetAllocator() const { return *m_pAllocator; }

	private:

		static EA::TagValuePair s_nullTVP;

		EA::TagValuePair* m_pTVP = nullptr;
		EA::Allocator::IAllocator* m_pAllocator = nullptr;
	};
}
