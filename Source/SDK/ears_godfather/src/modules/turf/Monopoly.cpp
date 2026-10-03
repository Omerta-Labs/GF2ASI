#include "Monopoly.h"

namespace EARS
{
	namespace Modules
	{
		String* Monopoly::GetDisplayName()
		{
			return &m_DisplayName;
		}

		String* Monopoly::GetPerkDescription()
		{
			return &m_PerkDescription;
		}
	}
}