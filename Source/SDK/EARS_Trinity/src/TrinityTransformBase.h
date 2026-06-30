#pragma once

#include "SDK/EARS_Trinity/src/Interpolator.h"

namespace EA::Trinity
{
	class TrinityTransformBase
	{
	public:

		uint32_t m_Flags = 0;
		//unsigned __int32 mFlagPad : 28;
		//unsigned __int32 mFocalLengthDone : 1;
		//unsigned __int32 mRotDone : 1;
		//unsigned __int32 mTransDone : 1;
		//unsigned __int32 mHaveSeperateChannels : 1;
		void* m_Sequence = nullptr;
		EA::Trinity::Interpolator m_TransInterpX;
		EA::Trinity::Interpolator m_TransInterpY;
		EA::Trinity::Interpolator m_TransInterpZ;
		EA::Trinity::Interpolator m_RotInterpX;
		EA::Trinity::Interpolator m_RotInterpY;
		EA::Trinity::Interpolator m_RotInterpZ;
	};

	static_assert(sizeof(TrinityTransformBase) == 0x68);
}
