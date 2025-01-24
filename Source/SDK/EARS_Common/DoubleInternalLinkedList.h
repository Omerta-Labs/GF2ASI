#pragma once

namespace EARS
{
	namespace Common
	{
		template <typename TType>
		class DoubleLinkedListNodeMixin
		{
		private:

			TType* m_Next = nullptr;
			TType* m_Prev = nullptr;
		};
	} // Common
} // EARS
