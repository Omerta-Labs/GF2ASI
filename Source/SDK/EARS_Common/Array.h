#pragma once

template <typename T>
struct Array
{
	void Add(T Object)
	{
		// Make sure we've got enough capacity
		if (m_Size == m_Capacity)
		{
			unsigned int NextCapacity = (m_Capacity ? 2 * m_Capacity : 1);
			Reserve(NextCapacity);
		}

		// Extend array and add new object
		const unsigned int Size = m_Size;
		T* Slot = &m_Items[Size];
		m_Size++;
		if (Slot)
		{
			*Slot = *Object;
		}
	}

	void Reserve(unsigned int InCapacity)
	{
		if (InCapacity > m_Capacity)
		{
			T* NewArr = new T[sizeof(T) * InCapacity];
			if (m_Items)
			{
				T* Itr = NewArr;
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

	T& operator[](unsigned int idx) { return m_Items[idx]; }
	const T& operator[](unsigned int idx) const { return m_Items[idx]; }

	T* m_Items;
	unsigned int m_Size;
	unsigned int m_Capacity;
};