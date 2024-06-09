#include "PlayerFamilyTree.h"

void EARS::Modules::PlayerFamilyTree::ForEachMember(const TVisitFamilyMemberFunctor& InFunction) const
{
	for (uint32_t i = 0; i < 7; i++)
	{
		InFunction(m_FamilyMembers[i]);
	}
}

EARS::Modules::PlayerFamilyTree* EARS::Modules::PlayerFamilyTree::GetInstance()
{
	return *(EARS::Modules::PlayerFamilyTree**)0x112989C;
}
