#include "ears_rt_cct/chrcntl_animstate.h"

namespace EA
{
	namespace CCT
	{
		namespace Internal
		{
			void AnimState::SetFlags(int32_t InFlags)
			{
				m_Flags |= InFlags;
			}

			void AnimState::ClearFlags(int32_t InFlags)
			{
				m_Flags &= ~InFlags;
			}
		} // Internal
	} // CCT
} // EA
