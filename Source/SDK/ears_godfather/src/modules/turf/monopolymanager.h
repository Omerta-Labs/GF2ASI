#pragma once

// SDK (Common)
#include "ears_common/array.h"
#include "ears_common/singleton.h"

namespace EARS
{
	namespace Modules
	{
		// foward declare
		class Monopoly;

		class MonopolyManager : public Singleton<MonopolyManager>
		{
		public:

			virtual ~MonopolyManager() { /* implemented in game */ }

			// Get the MonopolyManager instance
			static MonopolyManager* GetInstance();

		//private:

			bool m_bMessagesEnabled = false;

			Array<EARS::Modules::Monopoly*> m_Monopolies;
		};
	}
}
