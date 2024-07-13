#pragma once

// CPP
#include <stdint.h>

template <typename TType, int TNum>
class StaticArray
{
public:

	inline uint32_t GetCapacity() const { return TNum; }
	inline uint32_t GetSize() const { return m_Size; }

	TType& operator[](uint32_t idx) { return m_Items[idx]; }
	const TType& operator[](uint32_t idx) const { return m_Items[idx]; }

public:

	typedef TType* RangedForIteratorType;
	typedef const TType* RangedForConstIteratorType;

	inline RangedForIteratorType begin() { return &m_Items[0]; }
	inline RangedForConstIteratorType begin() const { return &m_Items[0]; }
	inline RangedForIteratorType end() { return &m_Items[0] + m_Size; }
	inline RangedForConstIteratorType end() const { return &m_Items[0] + m_Size; }

private:

	TType m_Items[TNum];
	uint32_t m_Size = 0;
};