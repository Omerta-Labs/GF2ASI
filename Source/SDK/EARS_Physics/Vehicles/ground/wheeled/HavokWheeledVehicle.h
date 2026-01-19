#pragma once

// SDK
#include "SDK/EARS_Common/Bitflags.h"

// C++
#include <stdint.h>

namespace EARS::Vehicles
{
	enum class HavokWheeledVehicleMode : int32_t
	{
		LINEAR_HAVOK_VEHICLE_MODE = 0x0,
		FULL_HAVOK_VEHICLE_MODE = 0x1,
		SIMPLE_HAVOK_VEHICLE_MODE = 0x2,
	};

	class HavokWheeledVehicle
	{
	public:

		void* vtable;
		char m_HavokWheeledVehicle[0x108];
		HavokWheeledVehicleMode m_Mode;
		Flags32 m_hkVehicleFlags;

	private:
	};
}
