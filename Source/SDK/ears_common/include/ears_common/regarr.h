#pragma once

#include "array.h"

// C++
#include <stdint.h>

/**
 * Associative array held in one flat, key-ascending allocation: a lookup is a
 * binary search rather than a hash or a node chase, and iteration comes out in
 * key order for free. Intended for tables registered once and read often.
 *
 * MAYBE_OPTIMISE: only the lookups are logarithmic. Insert and FindOrInsert
 * find the insertion point by scanning linearly from index 0 and then shuffle
 * every later element up one slot, so filling a table of n entries costs O(n^2)
 * key comparisons and element copies even though FindIndex right next to them
 * would locate the same slot in O(log n).
 */
template <typename TValue, typename TKey>
class RegArr
{
public:

	struct RegData
	{
		TKey m_Key;
		TValue m_Data;
	};

	RegArr() {}

	// InMaxElem is a capacity hint handed straight to the backing array; Size()
	// is still 0 afterwards.
	RegArr(uint32_t InMaxElem) : m_Arr(InMaxElem) {}

	// bInFreeData also releases the backing allocation, dropping Capacity() to
	// 0. False keeps the allocation for reuse and only resets the element count.
	void Clear(bool bInFreeData)
	{
		if (bInFreeData)
		{
			m_Arr.clear();
		}
		else
		{
			m_Arr.resize(0);
		}
	}

	/**
	 * Reserves a slot for InKey and returns a pointer to its value, which is raw
	 * memory the caller is expected to fill in.
	 *
	 * Returns nullptr, and changes nothing, when InKey is already registered: an
	 * existing entry is never replaced. Use FindOrInsert when the existing value
	 * is wanted instead.
	 */
	TValue* Insert(const TKey& InKey)
	{
		const int32_t ArraySize = m_Arr.Size();

		int32_t ObjIdx = 0;
		for (; ObjIdx < ArraySize; ObjIdx++)
		{
			RegData& Element = m_Arr[ObjIdx];
			if (InKey == Element.m_Key)
			{
				return nullptr;
			}

			// Keys ascend, so the first key above this one is where InKey belongs.
			if (InKey < Element.m_Key)
			{
				break;
			}
		}

		RegData& NewData = m_Arr.Insert(ObjIdx);
		NewData.m_Key = InKey;
		return &NewData.m_Data;
	}

	// Returns the existing value for InKey, or inserts InInsertedValueIfNotFound
	// and returns that. Never returns nullptr.
	TValue* FindOrInsert(const TKey& InKey, const TValue& InInsertedValueIfNotFound)
	{
		const int32_t ArraySize = m_Arr.Size();

		int32_t ObjIdx = 0;
		for (; ObjIdx < ArraySize; ObjIdx++)
		{
			RegData& Element = m_Arr[ObjIdx];
			if (InKey == Element.m_Key)
			{
				return &m_Arr[ObjIdx].m_Data;
			}

			if (InKey < Element.m_Key)
			{
				break;
			}
		}

		RegData& NewData = m_Arr.Insert(ObjIdx);
		NewData.m_Key = InKey;
		NewData.m_Data = InInsertedValueIfNotFound;
		return &NewData.m_Data;
	}

	// Returns false when InKey was not registered. Every index above the removed
	// entry shifts down by one, so indices obtained earlier are invalidated.
	bool Remove(const TKey& InKey)
	{
		bool bRemoved = false;

		const int32_t ObjIdx = FindIndex(InKey);
		if (ObjIdx >= 0)
		{
			m_Arr.Delete(ObjIdx);
			bRemoved = true;
		}

		return bRemoved;
	}

	// Returns false only for a negative InIdx. An InIdx at or past Size() is not
	// rejected here; the backing array asserts it in the debug build and runs off
	// the end of the live elements otherwise.
	bool RemoveIndex(int32_t InIdx)
	{
		bool bRemoved = false;

		if (InIdx >= 0)
		{
			m_Arr.Delete(InIdx);
			bRemoved = true;
		}

		return bRemoved;
	}

	// Returns nullptr when InKey is not registered. The pointer is into the
	// backing array, so any Insert, Remove or Clear invalidates it.
	const TValue* Search(const TKey& InKey) const
	{
		const int32_t Idx = FindIndex(InKey);
		if (Idx >= 0)
		{
			return &m_Arr[Idx].m_Data;
		}

		return nullptr;
	}

	TValue* Search(const TKey& InKey)
	{
		const int32_t Idx = FindIndex(InKey);
		if (Idx >= 0)
		{
			return &m_Arr[Idx].m_Data;
		}

		return nullptr;
	}

	uint32_t Size() const { return m_Arr.Size(); }
	uint32_t Capacity() const { return m_Arr.Capacity(); }

	// Indexes by position, not by key. Positions are key-ascending and are
	// invalidated by any insert or remove.
	TValue& operator[](int32_t InIdx) { return m_Arr[InIdx].m_Data; }
	const TValue& operator[](int32_t InIdx) const { return m_Arr[InIdx].m_Data; }

	/**
	 * Binary search for InKey, returning its index or -1 when it is absent.
	 *
	 * UpperIdx converges on the first element whose key is not below InKey (a
	 * lower bound), which is why the loop needs no equality test of its own and
	 * why LowerIdx starts at -1 rather than 0: the answer may be index 0.
	 */
	int32_t FindIndex(const TKey& InKey) const
	{
		int32_t FoundIdx = -1;

		const int32_t ArraySize = m_Arr.Size();

		int32_t LowerIdx = -1;
		int32_t UpperIdx = ArraySize;
		while (LowerIdx + 1 != UpperIdx)
		{
			const int32_t MidIdx = (LowerIdx + UpperIdx) >> 1;
			if (m_Arr[MidIdx].m_Key < InKey)
			{
				LowerIdx = MidIdx;
			}
			else
			{
				UpperIdx = MidIdx;
			}
		}

		if (UpperIdx < ArraySize)
		{
			const RegData& Element = m_Arr[UpperIdx];
			if (Element.m_Key == InKey)
			{
				FoundIdx = UpperIdx;
			}
		}

		return FoundIdx;
	}

	void GetKeyValuePairFromIndex(int32_t InIdx, TKey& OutSearchKey, TValue& OutValue) const
	{
		const RegData& Element = m_Arr[InIdx];
		OutSearchKey = Element.m_Key;
		OutValue = Element.m_Data;
	}

	const TKey& GetKeyFromIndex(int32_t InIdx) const { return m_Arr[InIdx].m_Key; }

	// DEVIATION: range-for support is ours; the original has no iterators and
	// callers indexed with Size()/operator[]. Iterating yields RegData, not
	// TValue, so the key stays reachable.
	typedef RegData* RangedForIteratorType;
	typedef const RegData* RangedForConstIteratorType;

	inline RangedForIteratorType begin() { return &m_Arr[0]; }
	inline RangedForConstIteratorType begin() const { return &m_Arr[0]; }
	inline RangedForIteratorType end() { return &m_Arr[0] + Size(); }
	inline RangedForConstIteratorType end() const { return &m_Arr[0] + Size(); }

private:

	// Declared and never defined: copying a RegArr is a link error rather than
	// an aliased allocation.
	RegArr(const RegArr& InOther);
	RegArr& operator=(const RegArr& InOther);

	Array<RegData> m_Arr;
};
