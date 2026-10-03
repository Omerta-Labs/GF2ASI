#pragma once

#include "Array.h"

template<typename TValue, typename TKey>
struct RegArr
{
public:

	struct RegData
	{
		TKey m_Key;
		TValue m_Data;
	};

	int32_t FindIndex(const TKey& KeyValue) const
	{
		for (uint32_t idx = 0; idx < Size(); idx++)
		{
			const RegData& Element = m_Items[idx];
			if (Element.m_Key == KeyValue)
			{
				return idx;
			}

			// The keys are sorted, so once its less than we can assume it does not exist
			if (Element.m_Key > KeyValue)
			{
				return -1;
			}
		}

		return -1;
	}

	TValue* Search(const TKey& KeyValue) const
	{
		const int32_t ObjIdx = FindIndex(KeyValue);
		if (ObjIdx != -1)
		{
			return &m_Items[ObjIdx].m_Data;
		}

		return nullptr;
	}

	inline uint32_t Size() const { return m_Items.Size(); }

	TValue& operator[](uint32_t idx) { return m_Items[idx].m_Data; }

	typedef RegData* RangedForIteratorType;
	typedef const RegData* RangedForConstIteratorType;

	inline RangedForIteratorType begin() { return &m_Items[0]; }
	inline RangedForConstIteratorType begin() const { return &m_Items[0]; }
	inline RangedForIteratorType end() { return &m_Items[0] + Size(); }
	inline RangedForConstIteratorType end() const { return &m_Items[0] + Size(); }

private:

	Array<RegArr<TValue, TKey>::RegData> m_Items;
};
