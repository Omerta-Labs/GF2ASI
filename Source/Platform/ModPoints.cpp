#include "Platform/ModPoints.h"

namespace EARS::ModPoints
{
	namespace
	{
		ResolveAttributePacketFn g_ResolveAttributePacket = nullptr;
	}

	void SetResolveAttributePacket(const ResolveAttributePacketFn Fn)
	{
		g_ResolveAttributePacket = Fn;
	}

	RWS::CAttributePacket* ResolveAttributePacket(const EARS::Common::guid128_t& PacketId)
	{
		if (g_ResolveAttributePacket == nullptr)
		{
			return nullptr;
		}

		return g_ResolveAttributePacket(PacketId);
	}
}
