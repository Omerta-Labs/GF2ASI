#pragma once

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
			Slot = new TType(Object);
		}
	}

	void Reserve(unsigned int InCapacity)
	{
		if (InCapacity > m_Capacity)
		{
			TType* NewArr = new TType[sizeof(TType) * InCapacity];
			if (m_Items)
			{
				TType* Itr = NewArr;
				for (unsigned int i = 0; i < m_Size; i++)
				{
					*Itr = m_Items[i];
					++Itr;
				}
			}

			m_Items = NewArr;
			m_Capacity = InCapacity;
		}
	}

	inline unsigned int Capacity() const { return m_Capacity; }
	inline unsigned int Size() const { return m_Size; }
	inline bool IsEmpty() const { return (Size() == 0); }

	TType& operator[](unsigned int idx) { return m_Items[idx]; }
	const TType& operator[](unsigned int idx) const { return m_Items[idx]; }

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