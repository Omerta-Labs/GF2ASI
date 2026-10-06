#pragma once

// SDK
#include "framework/core/memory/globalheapallocator.h"

// C++
#include <new>
#include <stdint.h>

template <typename TType>
struct Array
{
public:

	Array()
		: m_Items(nullptr)
		, m_Size(0)
		, m_Capacity(0)
	{
		// empty, does nothing
	}

	// Allocates InCapacity elements up front without constructing any of them,
	// so Size() is still 0. Does not go through Reserve.
	Array(uint32_t InCapacity)
		: m_Items((TType*)EARS::Allocator::GlobalHeapAllocator::OperatorNewArray(sizeof(TType) * InCapacity))
		, m_Size(0)
		, m_Capacity(InCapacity)
	{
	}

	~Array()
	{
		for (int32_t Idx = (int32_t)m_Size - 1; Idx >= 0; Idx--)
		{
			m_Items[Idx].~TType();
		}

		if (m_Items)
		{
			EARS::Allocator::GlobalHeapAllocator::OperatorDeleteArray(m_Items);
		}
	}

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

	void Reserve(uint32_t InCapacity)
	{
		if (InCapacity > m_Capacity)
		{			
			// TODO: Should be using new operator[]
			TType* NewArr = (TType*)EARS::Allocator::GlobalHeapAllocator::OperatorNewArray(sizeof(TType) * InCapacity);
			if (m_Items)
			{
				for (uint32_t i = 0; i < m_Size; i++)
				{
					new (&NewArr[i]) TType(m_Items[i]);
				}

				// TODO: Should be using delete operator[]
				EARS::Allocator::GlobalHeapAllocator::OperatorDeleteArray(m_Items);
			}

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

	// Unordered removal: the last element is moved into the hole. Cheaper than
	// Delete, but it reorders, so an index held across a DeleteFast is stale.
	void DeleteFast(uint32_t ObjectIdx)
	{
		if (ObjectIdx != (m_Size - 1))
		{
			m_Items[ObjectIdx] = m_Items[m_Size - 1];
		}

		m_Size--;
	}

	/**
	 * Opens a slot at ObjectIdx and returns it, shuffling every later element up
	 * one. ObjectIdx may equal Size(), which appends. The returned element is
	 * default-constructed; for a trivial TType that leaves it uninitialised.
	 */
	TType& Insert(uint32_t ObjectIdx)
	{
		if (m_Size == m_Capacity)
		{
			UpsizePwr2();
		}

		if (ObjectIdx == m_Size)
		{
			new (&m_Items[m_Size]) TType;
			return m_Items[m_Size++];
		}

		// The slot one past the end is raw memory, so the element moved into it
		// is copy-constructed; the rest of the shuffle is plain assignment.
		new (&m_Items[m_Size]) TType(m_Items[m_Size - 1]);

		for (uint32_t Idx = m_Size - 1; Idx > ObjectIdx; Idx--)
		{
			m_Items[Idx] = m_Items[Idx - 1];
		}

		m_Size++;

		new (&m_Items[ObjectIdx]) TType;
		return m_Items[ObjectIdx];
	}

	// Order-preserving removal: every later element shifts down one. Unlike
	// DeleteFast this never reorders, which is what a sorted array needs.
	void Delete(uint32_t ObjectIdx)
	{
		for (uint32_t Idx = ObjectIdx; Idx < (m_Size - 1); Idx++)
		{
			m_Items[Idx] = m_Items[Idx + 1];
		}

		m_Size--;
	}

	void Resize(uint32_t InSize)
	{
		if (InSize > m_Size)
		{
			Reserve(InSize);

			for (uint32_t Idx = m_Size; Idx < InSize; Idx++)
			{
				new (&m_Items[Idx]) TType;
			}

			m_Size = InSize;
		}
		else if (InSize < m_Size)
		{
			for (int32_t Idx = (int32_t)m_Size - 1; Idx >= (int32_t)InSize; Idx--)
			{
				m_Items[Idx].~TType();
			}

			m_Size = InSize;
		}
	}

	// Drops every element and the allocation with it, so Capacity() goes to 0.
	void clear()
	{
		for (int32_t Idx = (int32_t)m_Size - 1; Idx >= 0; Idx--)
		{
			m_Items[Idx].~TType();
		}

		m_Size = 0;
		EARS::Allocator::GlobalHeapAllocator::OperatorDeleteArray(m_Items);
		m_Items = nullptr;
		m_Capacity = 0;
	}

	void resize(uint32_t InSize) { Resize(InSize); }

	inline uint32_t Capacity() const { return m_Capacity; }
	inline uint32_t Size() const { return m_Size; }
	inline bool IsEmpty() const { return (Size() == 0); }

	TType& operator[](uint32_t idx) const { return m_Items[idx]; }

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
		const uint32_t NextCapacity = (m_Capacity ? 2 * m_Capacity : 1);
		Reserve(NextCapacity);
	}

	TType* m_Items;
	uint32_t m_Size = 0;
	uint32_t m_Capacity = 0;
};
