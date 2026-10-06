#pragma once

// SDK
#include "ears_common/tvpcontainer.h"

// C++
#include <new>
#include <stdint.h>
#include <stdlib.h>

/**
 * Growable array that allocates from a supplied EA::Allocator::IAllocator rather
 * than the global heap, carrying that allocator and a deep copy of its allocation
 * tag chain in m_AllocatorParms. Otherwise the same shape as Array<T>: a single
 * contiguous block, a size and a capacity, doubling on overflow.
 *
 * Every method has a lowercase STL-styled twin - Size/size, Clear/clear,
 * Add/push_back, Delete/erase, Reserve/reserve, Resize/resize, IsEmpty/empty,
 * Insert/insert - so the type can stand in for std::vector. Where the debug build
 * emitted both, the lowercase one forwards; see the notes on the few where it
 * emitted its own copy of the body instead.
 *
 * The allocator is reference-counted through TVPContainer, so an instance keeps
 * its allocator alive for as long as it holds memory.
 */
template <typename TClass>
struct ManagedArray
{
public:

	typedef TClass* iterator;
	typedef const TClass* const_iterator;

	// DEVIATION: the original has no default constructor - it has only the two
	// allocator-taking forms below, and the classes that hold a ManagedArray pass
	// an allocator from their own constructor's member-init list. Ours exists
	// because not every owning constructor is reconstructed yet.
	//
	// An instance built this way has no allocator and is only safe to leave alone:
	// Reserve will fault, and so will destroying it, because ~TVPContainer calls
	// Release on a null allocator. The destructor and Clear below skip the Free
	// when there is no block, which is why this does not fault the moment one goes
	// out of scope, but that is as far as the guard goes.
	ManagedArray()
		: m_Arr(nullptr)
		, m_Size(0)
		, m_Capacity(0)
	{
	}

	ManagedArray(EA::Allocator::IAllocator* InAllocator, const EA::TagValuePair& InTVP)
		: m_AllocatorParms(InTVP, InAllocator)
	{
		m_Arr = nullptr;
		m_Capacity = 0;
		m_Size = 0;
	}

	// UNVERIFIED: not instantiated. The capacity is reserved up front; the rest
	// matches the two-argument form.
	ManagedArray(uint32_t InCapacity, EA::Allocator::IAllocator* InAllocator, const EA::TagValuePair& InTVP)
		: m_AllocatorParms(InTVP, InAllocator)
	{
		m_Arr = nullptr;
		m_Capacity = 0;
		m_Size = 0;

		Reserve(InCapacity);
	}

	// UNVERIFIED: not instantiated. Copies the allocator parameters and then the
	// elements, so the copy owns its own block.
	ManagedArray(const ManagedArray& InOther)
		: m_AllocatorParms(InOther.m_AllocatorParms)
	{
		m_Arr = nullptr;
		m_Capacity = 0;
		m_Size = 0;

		Append(InOther.m_Arr, InOther.m_Size);
	}

	~ManagedArray()
	{
		for (int32_t Idx = (int32_t)m_Size - 1; Idx >= 0; Idx--)
		{
			m_Arr[Idx].~TClass();
		}

		// The original frees unconditionally; it can, because it always has an
		// allocator. See the note on the default constructor for why this one asks.
		if (m_Arr)
		{
			m_AllocatorParms.GetAllocator().Free(m_Arr, 0);
		}
	}

	void Reserve(uint32_t InCapacity)
	{
		if (InCapacity > m_Capacity)
		{
			TClass* pNewArr = (TClass*)m_AllocatorParms.GetAllocator().Alloc(sizeof(TClass) * InCapacity,
				m_AllocatorParms.Get());

			if (m_Arr)
			{
				for (uint32_t Idx = 0; Idx < m_Size; Idx++)
				{
					new (&pNewArr[Idx]) TClass(m_Arr[Idx]);
				}

				m_AllocatorParms.GetAllocator().Free(m_Arr, 0);
			}

			m_Arr = pNewArr;
			m_Capacity = InCapacity;
		}
	}

	void Resize(uint32_t InSize)
	{
		if (InSize > m_Size)
		{
			Reserve(InSize);

			for (uint32_t Idx = m_Size; Idx < InSize; Idx++)
			{
				new (&m_Arr[Idx]) TClass;
			}

			m_Size = InSize;
		}
		else if (InSize < m_Size)
		{
			for (int32_t Idx = (int32_t)m_Size - 1; Idx >= (int32_t)InSize; Idx--)
			{
				m_Arr[Idx].~TClass();
			}

			m_Size = InSize;
		}
	}

	uint32_t Size() const { return m_Size; }

	bool IsEmpty() const { return (Size() == 0); }

	uint32_t Capacity() const { return m_Capacity; }

	// UNVERIFIED: not instantiated. Appends a default-constructed element and
	// returns it, so the caller can fill it in place.
	TClass& Add()
	{
		Resize(m_Size + 1);
		return m_Arr[m_Size - 1];
	}

	void Add(const TClass& InObj)
	{
		if (m_Size == m_Capacity)
		{
			UpsizePwr2();
		}

		// The slot is addressed before m_Size moves, so InObj may be an element of
		// this same array as long as no reallocation happened above.
		TClass* pSlot = &m_Arr[m_Size];
		m_Size++;
		new (pSlot) TClass(InObj);
	}

	/**
	 * Opens a slot at InObjIdx and returns it, shuffling every later element up
	 * one. InObjIdx may equal Size(), which appends.
	 *
	 * UNVERIFIED: not instantiated for this element type. Taken from Array<T>'s
	 * Insert, which is the same routine in the sibling header and whose own body
	 * was read out of the debug build.
	 */
	TClass& Insert(uint32_t InObjIdx)
	{
		if (m_Size == m_Capacity)
		{
			UpsizePwr2();
		}

		if (InObjIdx == m_Size)
		{
			new (&m_Arr[m_Size]) TClass;
			return m_Arr[m_Size++];
		}

		// The slot one past the end is raw memory, so the element moved into it is
		// copy-constructed; the rest of the shuffle is plain assignment.
		new (&m_Arr[m_Size]) TClass(m_Arr[m_Size - 1]);

		for (uint32_t Idx = m_Size - 1; Idx > InObjIdx; Idx--)
		{
			m_Arr[Idx] = m_Arr[Idx - 1];
		}

		m_Size++;

		new (&m_Arr[InObjIdx]) TClass;
		return m_Arr[InObjIdx];
	}

	void Insert(const TClass& InObj, uint32_t InObjIdx)
	{
		TClass& Slot = Insert(InObjIdx);
		Slot = InObj;
	}

	// Appends InCount copies of InObj, reserving once for the whole run.
	void Append(const TClass& InObj, uint32_t InCount)
	{
		if (InCount)
		{
			Reserve(m_Size + InCount);

			const uint32_t TargetSize = m_Size + InCount;
			do
			{
				TClass* pSlot = &m_Arr[m_Size];
				m_Size++;
				new (pSlot) TClass(InObj);
			}
			while (TargetSize != m_Size);
		}
	}

	// UNVERIFIED: not instantiated. Appends InCount elements read from InObjs.
	void Append(const TClass* InObjs, uint32_t InCount)
	{
		if (InCount)
		{
			Reserve(m_Size + InCount);

			for (uint32_t Idx = 0; Idx < InCount; Idx++)
			{
				TClass* pSlot = &m_Arr[m_Size];
				m_Size++;
				new (pSlot) TClass(InObjs[Idx]);
			}
		}
	}

	// Unordered removal: the last element is moved into the hole. Cheaper than
	// Delete, but it reorders, so an index held across a DeleteFast is stale.
	void DeleteFast(uint32_t InObjIdx)
	{
		if (InObjIdx != (m_Size - 1))
		{
			m_Arr[InObjIdx] = m_Arr[m_Size - 1];
		}

		m_Size--;
	}

	// Order-preserving removal: every later element shifts down one.
	void Delete(uint32_t InObjIdx)
	{
		for (uint32_t Idx = InObjIdx; Idx < (m_Size - 1); Idx++)
		{
			m_Arr[Idx] = m_Arr[Idx + 1];
		}

		m_Size--;
	}

	// UNVERIFIED: not instantiated. Removes InCount elements from InObjIdx,
	// order-preserving.
	void Delete(const uint32_t InObjIdx, const uint32_t InCount)
	{
		for (uint32_t Idx = InObjIdx; Idx < (m_Size - InCount); Idx++)
		{
			m_Arr[Idx] = m_Arr[Idx + InCount];
		}

		m_Size -= InCount;
	}

	// Drops every element and the allocation with it, so Capacity() goes to 0.
	//
	// UNVERIFIED in one respect: the debug build emits a full body for Clear and
	// another for clear rather than one forwarding to the other, so which of the
	// two the original wrote out is not recoverable. They behave identically.
	void Clear()
	{
		for (int32_t Idx = (int32_t)m_Size - 1; Idx >= 0; Idx--)
		{
			m_Arr[Idx].~TClass();
		}

		if (m_Arr)
		{
			m_AllocatorParms.GetAllocator().Free(m_Arr, 0);
		}

		m_Arr = nullptr;
		m_Capacity = 0;
		m_Size = 0;
	}

	// UNVERIFIED: not instantiated, so only the signature is recovered - it takes
	// a comparison function pointer. The sort itself is inferred entirely.
	void QSort(int (*InCompare)(const void*, const void*))
	{
		if (m_Size > 1)
		{
			qsort(m_Arr, m_Size, sizeof(TClass), InCompare);
		}
	}

	// Both overloads require m_Arr non-null and InIdx below Size(); the original
	// asserts `m_arr && (idx < m_Size)` rather than clamping.
	TClass& operator[](uint32_t InIdx) { return m_Arr[InIdx]; }
	const TClass& operator[](uint32_t InIdx) const { return m_Arr[InIdx]; }

	// Returns the index of the first element equal to InObj, or -1.
	int32_t Find(const TClass& InObj) const
	{
		int32_t FoundIdx = -1;

		for (uint32_t Idx = 0; Idx < m_Size; Idx++)
		{
			if (m_Arr[Idx] == InObj)
			{
				FoundIdx = Idx;
				break;
			}
		}

		return FoundIdx;
	}

	// Null for an empty array even when a block is still reserved: the guard is on
	// Size(), not on the pointer.
	TClass* Data() { return (m_Size ? m_Arr : nullptr); }
	const TClass* Data() const { return (m_Size ? m_Arr : nullptr); }

	// UNVERIFIED: not instantiated. Exchanges the blocks of two arrays.
	void SwapData(ManagedArray& InOther)
	{
		TClass* pArr = m_Arr;
		const uint32_t ArrSize = m_Size;
		const uint32_t ArrCapacity = m_Capacity;

		m_Arr = InOther.m_Arr;
		m_Size = InOther.m_Size;
		m_Capacity = InOther.m_Capacity;

		InOther.m_Arr = pArr;
		InOther.m_Size = ArrSize;
		InOther.m_Capacity = ArrCapacity;
	}

	const EARS::Common::TVPContainer& GetAllocationData() const { return m_AllocatorParms; }

	// UNVERIFIED: not instantiated. Keeps this array's own allocator and replaces
	// its contents.
	ManagedArray& operator=(const ManagedArray& InOther)
	{
		if (this != &InOther)
		{
			Resize(0);
			Append(InOther.m_Arr, InOther.m_Size);
		}

		return *this;
	}

	const_iterator begin() const { return m_Arr; }
	iterator begin() { return m_Arr; }
	const_iterator end() const { return m_Arr + m_Size; }
	iterator end() { return m_Arr + m_Size; }

	void clear() { Clear(); }

	void push_back(const TClass& InObj) { Add(InObj); }

	// Requires a non-empty array; the original asserts `m_arr && m_Size`.
	void pop_back()
	{
		m_Size--;
		m_Arr[m_Size].~TClass();
	}

	// UNVERIFIED: not instantiated. Inserts before InIt.
	void insert(iterator InIt, const TClass& InObj) { Insert(InObj, (uint32_t)(InIt - m_Arr)); }

	void reserve(uint32_t InCapacity) { Reserve(InCapacity); }

	void resize(uint32_t InSize) { Resize(InSize); }

	// UNVERIFIED: not instantiated. Grows with copies of InObj rather than
	// default-constructed elements.
	void resize(uint32_t InSize, const TClass& InObj)
	{
		if (InSize > m_Size)
		{
			Append(InObj, InSize - m_Size);
		}
		else
		{
			Resize(InSize);
		}
	}

	// Shifts everything after InIt down one. Unlike Delete this works off the
	// iterator and leaves the trailing element's destructor uncalled, matching the
	// original. The original asserts `it`.
	void erase(iterator InIt)
	{
		iterator pLast = (m_Arr + m_Size) - 1;
		while (InIt < pLast)
		{
			*InIt = *(InIt + 1);
			InIt++;
		}

		m_Size--;
	}

	// UNVERIFIED: not instantiated for this element type.
	TClass& front() { return m_Arr[0]; }
	const TClass& front() const { return m_Arr[0]; }

	// UNVERIFIED: not instantiated for this element type.
	TClass& back() { return m_Arr[m_Size - 1]; }
	const TClass& back() const { return m_Arr[m_Size - 1]; }

	uint32_t size() const { return m_Size; }

	// Reads m_Size directly where IsEmpty goes through Size(); the two are
	// separate one-liners in the original rather than one calling the other.
	bool empty() const { return (m_Size == 0); }

	uint32_t capacity() const { return m_Capacity; }

private:

	void UpsizePwr2()
	{
		const uint32_t NextCapacity = (m_Capacity ? 2 * m_Capacity : 1);
		Reserve(NextCapacity);
	}

	TClass* m_Arr;
	uint32_t m_Size;
	uint32_t m_Capacity;
	EARS::Common::TVPContainer m_AllocatorParms;
};
