#pragma once

// C++
#include <stdint.h>

namespace EARS::Framework
{
    struct OffsetGUID_t
    {
        int32_t m_ByteOffsetFromThis = 0;
    };

    // EARS's bespoke packet structure inside a CAttributePacket
	struct EntityPacket
	{
        
        uint8_t m_AttrHandlerFlags = 0;
        uint8_t m_SpawnMask = 0;
        uint8_t m_AttachedRes = 0;
        uint8_t m_AttrPackets = 0;
        int32_t m_ComponentListOffset = 0;
        class EARS::Framework::OffsetGUID_t m_Guid;
        uint32_t m_BehaviorID = 0;
	};
} // EARS::Framework
