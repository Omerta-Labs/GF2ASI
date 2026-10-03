#include "GroupManager.h"

namespace EARS::Framework
{
	const GroupManager::TEntityList& GroupManager::FindGroupMembers(uint32_t GroupHandle) const
	{
		return m_GroupContainer[GroupHandle].m_MemberList;
	}

	GroupManager* GroupManager::GetInstance()
	{
		return *(GroupManager**)0x1223414;
	}

	bool GroupManager::IsGroupHandleInRange(uint32_t GroupHandle) const
	{
		return (GroupHandle < 0x40);
	}
}
