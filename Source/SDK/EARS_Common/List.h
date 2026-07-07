#pragma once

// SDK
#include "SDK/EARS_Common/TVPContainer.h"

// CPP
#include <stdint.h>

// forward dec
template<typename T>
struct List;

template<typename T>
struct ListNode
{
public:

	ListNode(const T& InItem)
	{
		m_Item = InItem;
		m_Next = m_Prev = nullptr;
	}

	const T& GetItem() const { return m_Item; }
	T& GetItem() { return m_Item; }
	const T* GetItemPtr() const { return &m_Item; }

	const ListNode<T>* GetNext() const { return m_Next; }
	const ListNode<T>* GetPrev() const { return m_Prev; }

private:

	// The owning list links nodes directly
	friend struct List<T>;

	T m_Item;
	ListNode<T>* m_Next;
	ListNode<T>* m_Prev;
};

/**
 * Doubly-linked list which allocates its nodes through the allocator
 * (and allocation tags) supplied on construction.
 */
template<typename T>
struct List
{
public:

	class ConstIterator
	{
	public:

		ConstIterator(const ListNode<T>* pNode)
		{
			m_pNode = pNode;
		}

		ConstIterator& operator++()
		{
			m_pNode = m_pNode ? m_pNode->GetNext() : nullptr;
			return *this;
		}

		ConstIterator& operator--()
		{
			m_pNode = m_pNode ? m_pNode->GetPrev() : nullptr;
			return *this;
		}

		bool operator==(const ConstIterator& InIter) { return m_pNode == InIter.m_pNode; }
		bool operator!=(const ConstIterator& InIter) { return m_pNode != InIter.m_pNode; }

		const T& operator*() const { return m_pNode->GetItem(); }
		const T* operator->() const { return m_pNode->GetItemPtr(); }

	protected:

		// Erase needs to unlink the node the iterator points at
		friend struct List<T>;

		const ListNode<T>* m_pNode;
	};

	class Iterator : public ConstIterator
	{
	public:

		Iterator(const ListNode<T>* pNode)
			: ConstIterator(nullptr)
		{
			this->m_pNode = pNode;
		}

		T& operator*() { return const_cast<ListNode<T>*>(this->m_pNode)->GetItem(); }
	};

	List(EA::Allocator::IAllocator* InAllocator, const EA::TagValuePair& InTVP)
		: m_AllocatorParams(InTVP, InAllocator)
	{
		m_pHead = m_pTail = nullptr;
		m_nItems = 0;
	}

	List(const List<T>& InSrc)
		: m_AllocatorParams(InSrc.m_AllocatorParams)
	{
		m_pHead = m_pTail = nullptr;
		m_nItems = 0;
		Construct(InSrc);
	}

	~List()
	{
		ListNode<T>* pNode = m_pHead;
		while (pNode)
		{
			ListNode<T>* pNext = pNode->m_Next;
			pNode->~ListNode();
			m_AllocatorParams.GetAllocator().Free(pNode, 0);
			pNode = pNext;
		}
	}

	Iterator Begin() { return Iterator(m_pHead); }
	ConstIterator Begin() const { return ConstIterator(m_pHead); }
	ConstIterator End() const { return ConstIterator(nullptr); }
	Iterator Last() { return Iterator(m_pTail); }

	int32_t Size() const { return m_nItems; }

	void Append(const T& InItem)
	{
		EA::Allocator::IAllocator& Allocator = m_AllocatorParams.GetAllocator();
		ListNode<T>* pNewNode = static_cast<ListNode<T>*>(Allocator.Alloc(sizeof(ListNode<T>), m_AllocatorParams.Get()));
		new (pNewNode) ListNode<T>(InItem);

		if (m_pTail)
		{
			m_pTail->m_Next = pNewNode;
			m_pTail->m_Next->m_Prev = m_pTail;
			m_pTail = m_pTail->m_Next;
		}
		else
		{
			m_pTail = pNewNode;
			m_pHead = pNewNode;
		}

		m_nItems++;
	}

	void Prepend(const T& InItem)
	{
		EA::Allocator::IAllocator& Allocator = m_AllocatorParams.GetAllocator();
		ListNode<T>* pNewHead = static_cast<ListNode<T>*>(Allocator.Alloc(sizeof(ListNode<T>), m_AllocatorParams.Get()));
		new (pNewHead) ListNode<T>(InItem);

		pNewHead->m_Next = m_pHead;
		if (m_pHead)
		{
			m_pHead->m_Prev = pNewHead;
		}
		m_pHead = pNewHead;

		if (!m_pTail)
		{
			m_pTail = m_pHead;
		}

		m_nItems++;
	}

	// Unlinks the node the iterator points at, advancing past it.
	// Returns an iterator to the element after the erased one.
	Iterator Erase(Iterator& InIter)
	{
		ListNode<T>* pNode = const_cast<ListNode<T>*>(InIter.m_pNode);
		++InIter;

		if (pNode == m_pHead)
		{
			if (pNode == m_pTail)
			{
				m_pTail = nullptr;
				m_pHead = nullptr;
			}
			else
			{
				m_pHead = pNode->m_Next;
				if (m_pHead)
				{
					m_pHead->m_Prev = nullptr;
				}
			}
		}
		else if (pNode == m_pTail)
		{
			m_pTail = pNode->m_Prev;
			m_pTail->m_Next = nullptr;
		}
		else
		{
			pNode->m_Prev->m_Next = pNode->m_Next;
			pNode->m_Next->m_Prev = pNode->m_Prev;
		}

		pNode->~ListNode();
		m_AllocatorParams.GetAllocator().Free(pNode, 0);
		m_nItems--;

		return InIter;
	}

	// Removes every occurrence of the item, front to back
	void Remove(const T& InItem)
	{
		Iterator Iter = Begin();
		while (Iter != End())
		{
			if (*Iter == InItem)
			{
				Iter = Erase(Iter);
			}
			else
			{
				++Iter;
			}
		}
	}

	// Removes the first occurrence of the item, searching back to front
	void RemoveOnceReverse(const T& InItem)
	{
		Iterator Iter = Last();
		while (Iter != End())
		{
			if (*Iter == InItem)
			{
				Iter = Erase(Iter);
				break;
			}
			--Iter;
		}
	}

	void Clear()
	{
		while (m_pHead)
		{
			m_pTail = m_pHead->m_Next;
			m_pHead->~ListNode();
			m_AllocatorParams.GetAllocator().Free(m_pHead, 0);
			m_pHead = m_pTail;
		}

		m_pHead = m_pTail = nullptr;
		m_nItems = 0;
	}

	// STL-style interface
	Iterator begin() { return Begin(); }
	ConstIterator begin() const { return Begin(); }
	ConstIterator end() const { return End(); }
	uint32_t size() const { return m_nItems; }
	void clear() { Clear(); }
	void push_back(const T& InItem) { Append(InItem); }
	void push_front(const T& InItem) { Prepend(InItem); }
	Iterator erase(Iterator& InIter) { return Erase(InIter); }

	void pop_front()
	{
		if (m_pHead)
		{
			ListNode<T>* pNode = m_pHead;
			if (pNode == m_pTail)
			{
				m_pTail = nullptr;
				m_pHead = nullptr;
			}
			else
			{
				m_pHead = pNode->m_Next;
				if (m_pHead)
				{
					m_pHead->m_Prev = nullptr;
				}
			}

			pNode->~ListNode();
			m_AllocatorParams.GetAllocator().Free(pNode, 0);
			m_nItems--;
		}
	}

private:

	// Deep-copies the source list into this (expects an empty list)
	void Construct(const List<T>& InSrc)
	{
		for (ConstIterator Iter = InSrc.Begin(); Iter != InSrc.End(); ++Iter)
		{
			Append(*Iter);
		}
	}

	ListNode<T>* m_pHead;
	ListNode<T>* m_pTail;
	int32_t m_nItems;
	EARS::Common::TVPContainer m_AllocatorParams;
};
