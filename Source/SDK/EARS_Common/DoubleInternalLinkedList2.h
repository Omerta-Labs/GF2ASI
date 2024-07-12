#pragma once

namespace EARS
{
	namespace Common
	{
		template <typename T>
		class DoubleLinkedListNodeMixin2
		{
		private:

			T** m_PrevNext = nullptr;
			T* m_Next = nullptr;
		};
	} // Common
} // EARS
