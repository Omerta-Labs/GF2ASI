#pragma once

// SDK Common
#include "SDK/EARS_Common/DoubleInternalLinkedList2.h"
#include "SDK/EARS_Common/Guid.h"

namespace RWS
{
	class CAttributeHandler : public EARS::Common::DoubleLinkedListNodeMixin2<CAttributeHandler>
	{
	public:

		virtual ~CAttributeHandler() = 0;

	private:

		// TODO: Figure out whether or not this is correct
		char m_AttributeHandler_Padding[0x30];
	};
} // EARS
