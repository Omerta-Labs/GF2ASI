#include "sentient.h"

#include "Platform/MemUtils.h"

namespace EARS
{
	namespace Modules
	{
		void Sentient::DeactivateHeadTracking(bool bForceDeactivate)
		{
			MemUtils::CallClassMethod<void, Sentient*, bool>(0x07F6300, this, bForceDeactivate);
		}
	} // Modules
} // EARS
