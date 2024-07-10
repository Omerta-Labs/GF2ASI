#pragma once

// SDK Common
#include "SDK/EARS_Common/Guid.h"

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

		// TODO: Figure out whether or not this is correct
		void* VTable = nullptr;
		//char m_AttribHandler_Pad0[16];
		//uint32_t m_StreamHandle = 0;
		//EARS::Common::guid128_t m_InstanceID;
	};
} // EARS
