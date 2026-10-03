#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Animated/Animated.h"

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
