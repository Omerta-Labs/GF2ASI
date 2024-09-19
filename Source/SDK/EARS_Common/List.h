#pragma once

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Common
	{
		template<typename T>
		struct ListNode
		{
		public:

		private:

			T m_Item = nullptr;
			ListNode<T>* m_Next = nullptr;
			ListNode<T>* m_Prev = nullptr;
		};

		template<typename T>
		struct List
		{
		public:

		private:

			ListNode<T>* m_Head = nullptr;
			ListNode<T>* m_Tail = nullptr;
			int32_t m_Items = 0;

			// TVPContainer
			void* m_TVP = nullptr;
			void* m_Allocator = nullptr;
		};
	}
}
