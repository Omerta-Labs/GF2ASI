#include "Sentient.h"

// Hook
#include <Addons/Hook.h>

void EARS::Modules::Sentient::Translate(const float X, const float Y, const float Z)
{
	struct RwV3d
	{
		float x, y, z;
	};

	MemUtils::CallClassMethod<void, EARS::Modules::Sentient*, const RwV3d&>(0x07FF500, this, RwV3d(X, Y, Z));
}
