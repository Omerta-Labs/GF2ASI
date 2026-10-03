#include "Component.h"

namespace EARS::Framework
{
	void ComponentListRecord::EnableMessagesToComponents(EARS::Framework::Component** Components)
	{
		for (uint32_t idx = 0; idx < m_NumComponents; idx++)
		{
			Components[idx]->EnableMessages();
		}
	}

	void ComponentListRecord::DisableMessagesToComponents(EARS::Framework::Component** Components)
	{
		for (uint32_t idx = 0; idx < m_NumComponents; idx++)
		{
			Components[idx]->DisableMessages();
		}
	}
}
