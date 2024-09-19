#pragma once

// SDK (Common)
#include "SDK/EARS_Common/List.h"

namespace EARS
{
	namespace Framework
	{
		// forward decs
		class Entity;

		/**
		 * The Group Manager is Godfather II allows Entities to be associated in Groups
		 */
		class GroupManager
		{
		public:

			virtual ~GroupManager();

			static GroupManager* GetInstance();

		private:

			struct GroupContainerElement
			{
				uint32_t m_GroupID = 0;
				uint32_t m_NumTimesReserved = 0;
				EARS::Common::List<EARS::Framework::Entity*> m_Items;
			};

			// NB: Max number of groups is 64
			GroupContainerElement* m_GroupContainer = nullptr;
			void* m_MemberListAllocator = nullptr;					//  EARS::Framework::FastPoolIAllocator
		};
	}
}
