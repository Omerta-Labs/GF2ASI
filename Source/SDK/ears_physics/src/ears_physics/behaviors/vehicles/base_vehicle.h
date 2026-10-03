#pragma once

// SDK
#include "SDK/ears_framework/src/framework/core/animated/animated.h"

namespace EARS::Vehicles
{
	class BaseVehicle : public EARS::Framework::Animated
	{
	public:

		struct VehicleLocator
		{
			uint32_t m_LocatorID = 0;
			EARS::Framework::Entity::BoneInfo m_BoneInfo;
		};

	private:
	};
}
