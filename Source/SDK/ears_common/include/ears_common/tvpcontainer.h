#pragma once

// SDK
#include "allocator/iallocator.h"

namespace EARS::Common
{
	/**
	 * Holds the allocator a container should allocate from, together with a
	 * deep copy of the allocation tag chain (EA::TagValuePair) it was given.
	 */
	struct TVPContainer
	{
	public:

		TVPContainer()
			: m_TVP(nullptr)
			, m_Allocator(nullptr)
		{

		}

		TVPContainer(const EA::TagValuePair& InTVP, EA::Allocator::IAllocator* InAllocator)
		{
			m_TVP = CopyTVP(InTVP, InAllocator);
			m_Allocator = InAllocator;
			m_Allocator->AddRef();
		}

		TVPContainer(const TVPContainer& InOther)
		{
			m_TVP = InOther.m_TVP ? CopyTVP(*InOther.m_TVP, InOther.m_Allocator) : nullptr;
			m_Allocator = InOther.m_Allocator;
			m_Allocator->AddRef();
		}

		~TVPContainer()
		{
			if (m_TVP)
			{
				m_Allocator->Free(m_TVP, 0);
			}

			m_Allocator->Release();
		}

		// Deep-copies a tag chain into one contiguous block allocated from the
		// given allocator. Returns null for an empty (default) tag.
		static EA::TagValuePair* CopyTVP(const EA::TagValuePair& InTVP, EA::Allocator::IAllocator* InAllocator);

		const EA::TagValuePair& Get() const { return m_TVP ? *m_TVP : s_nullTVP; }
		EA::Allocator::IAllocator& GetAllocator() const { return *m_Allocator; }

	private:

		static EA::TagValuePair s_nullTVP;

		EA::TagValuePair* m_TVP = nullptr;
		EA::Allocator::IAllocator* m_Allocator = nullptr;
	};
}
