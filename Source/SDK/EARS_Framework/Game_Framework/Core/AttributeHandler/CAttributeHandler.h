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

		/**
		 * Fetch the Instance ID of this Attribute Handler.
		 */
		EARS::Common::guid128_t InqInstanceID() const { return m_InstanceId; }

	private:

		RWS::CAttributeHandler** m_PrevNextHandlerFromPacket = nullptr;
		RWS::CAttributeHandler* m_NextHandlerFromPacket = nullptr;
		uint32_t m_FlagsAndID = 0;
		uint32_t m_SubID = 0;
		uint32_t m_hStream = 0;
		EARS::Common::guid128_t m_InstanceId;

		// TODO: Figure out whether or not this is correct
		char m_AttributeHandler_Padding[0xC];
	};
} // EARS
