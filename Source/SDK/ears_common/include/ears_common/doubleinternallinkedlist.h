#pragma once

namespace EARS
{
	namespace Common
	{
		template <typename TType>
		class DoubleLinkedListNodeMixin
		{
		public:

			TType* GetNext() const { return m_Next; }
			void SetNext(TType* Next) { m_Next = Next; }

			TType* GetPrev() const { return m_Prev; }
			void SetPrev(TType* Prev) { m_Prev = Prev; }

		private:

			TType* m_Next = nullptr;
			TType* m_Prev = nullptr;
		};

		template <typename TType>
		struct DoubleInternalLinkedList
		{
		public:

			void InsertAtBack(TType* NodeToInsert)
			{
				NodeToInsert->SetNext(nullptr);
				NodeToInsert->SetPrev(m_Tail);

				if (m_Tail)
				{
					m_Tail->SetNext(NodeToInsert);
				}
				else
				{
					m_Head = NodeToInsert;
				}

				m_Tail = NodeToInsert;
			}

			bool IsEmpty() const { return (m_Head == nullptr); }

			TType* GetFront() const { return m_Head; }

		private:

			TType* m_Head = nullptr;
			TType* m_Tail = nullptr;
		};

		template <typename TType>
		struct DoubleInternalLinkedList2
		{
		public:

			TType* GetFront() const { return m_Head; }

		private:

			TType* m_Head = nullptr;
		};
	} // Common
} // EARS
