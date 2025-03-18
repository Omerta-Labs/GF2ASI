#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Entity/Entity.h"

// forward declares
namespace EA
{
	namespace CCT
	{
		class AnimView;
	}
}

namespace EARS
{
	namespace Framework
	{
		class Animated : public Entity
		{
		public:

			// getters
			EA::CCT::AnimView* GetAnimViewInfo() const { return m_AnimViewInfo; }

			/**
			 * Fetch the Model Stream Guid (as guid128_t) from the desired AttributePacket.
			 * Once retrieved, you can convert the guid128_t to guid32_t to find the Stream File GUID
			 * @param AttributePacket - The Packet to use to search for the guid
			 * @return EARS::Common::guid128_t - Stream GUID found within the Packet
			 */
			static EARS::Common::guid128_t FindModelStreamGuid(const RWS::CAttributePacket& AttributePacket);

		private:

			char m_Animated_Padding[0x8C];
			void* m_CctInfo = nullptr;		// 0x20C
			RWS::CEventId m_MsgSetAnimTrigger;
			EA::CCT::AnimView* m_AnimViewInfo = nullptr;
			char m_Animated_Padding2[0x94];
		};

		static_assert(sizeof(Animated) == 0x2B0, "EARS::Framework::Animated must equal 0x2B0");
	} // Modules
} // EARS
