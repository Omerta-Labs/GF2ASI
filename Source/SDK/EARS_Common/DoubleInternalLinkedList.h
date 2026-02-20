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

		private:

			TType* m_Next = nullptr;
			TType* m_Prev = nullptr;
		};

		template <typename TType>
		struct DoubleInternalLinkedList
		{
		public:

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
