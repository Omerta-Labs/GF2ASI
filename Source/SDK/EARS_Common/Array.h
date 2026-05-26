#pragma once

// SDK
#include "SDK/EARS_Godfather/System/Memory/GlobalHeapAllocator.h"

// C++
#include <stdint.h>

template <typename TType>
struct Array
{
public:

	void Add(const TType& Object)
	{
		// Make sure we've got enough capacity
		if (m_Size == m_Capacity)
		{
			UpsizePwr2();
		}

		// Extend array and add new object
		TType* Slot = &m_Items[m_Size++];
		if (Slot)
		{
			*Slot = Object;
		}
	}

	void Reserve(unsigned int InCapacity)
	{
		if (InCapacity > m_Capacity)
		{			
			// TODO: Should be using new operator[]
			TType* NewArr = (TType*)EARS::Allocator::GlobalHeapAllocator::OperatorNewArray(4 * InCapacity);
			if (m_Items)
			{
				TType* Itr = NewArr;
				for (unsigned int i = 0; i < m_Size; i++)
				{
					*Itr = m_Items[i];
					++Itr;
				}
			}

			// TODO: Should be using delete operator[]
			EARS::Allocator::GlobalHeapAllocator::OperatorDeleteArray(m_Items);
			m_Items = NewArr;
			m_Capacity = InCapacity;
		}
	}

	int32_t Find(const TType& Element) const
	{
		for (uint32_t Idx = 0; Idx < m_Size; Idx++)
		{
			if (m_Items[Idx] == Element)
			{
				return Idx;
			}
		}

		return -1;
	}

	void DeleteFast(uint32_t ObjectIdx)
	{
		if (ObjectIdx != (m_Size - 1))
		{
			m_Items[ObjectIdx] = m_Items[m_Size - 1];
			m_Size--;
		}
	}

	inline unsigned int Capacity() const { return m_Capacity; }
	inline unsigned int Size() const { return m_Size; }
	inline bool IsEmpty() const { return (Size() == 0); }

	TType& operator[](unsigned int idx) const { return m_Items[idx]; }

public:

	typedef TType* RangedForIteratorType;
	typedef const TType* RangedForConstIteratorType;

	inline RangedForIteratorType begin() { return &m_Items[0]; }
	inline RangedForConstIteratorType begin() const { return &m_Items[0]; }
	inline RangedForIteratorType end() { return &m_Items[0] + m_Size; }
	inline RangedForConstIteratorType end() const { return &m_Items[0] + m_Size; }

private:

	void UpsizePwr2()
	{
		const unsigned int NextCapacity = (m_Capacity ? 2 * m_Capacity : 1);
		Reserve(NextCapacity);
	}

	TType* m_Items;
	unsigned int m_Size;
	unsigned int m_Capacity;
};