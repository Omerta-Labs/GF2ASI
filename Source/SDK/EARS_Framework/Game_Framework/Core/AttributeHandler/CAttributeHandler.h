#pragma once

template <typename T>
class DoubleLinkedListNodeMixin2
{
private:

	T** m_PrevNext = nullptr;
	T* m_Next = nullptr;
};

namespace RWS
{
	class CAttributeHandler : public DoubleLinkedListNodeMixin2<CAttributeHandler>
	{
	public:

	private:

		void* VTable = nullptr;
	};
} // EARS
