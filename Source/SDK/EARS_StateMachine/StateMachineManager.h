#pragma once

// SDK
#include "SDK/EARS_Common/Singleton.h"

// C++
#include <stdint.h>

// forwar declares
namespace EA
{
	namespace Allocator
	{
		class IAllocator;
	}
}

namespace EARS
{
	namespace StateMachineSys
	{
		struct StateTable;

		class StateMachineManager : public Singleton<StateMachineManager>
		{
		public:

			virtual ~StateMachineManager() { /* implemented in engine */ }

			/* Fetch State Table using Given ID */
			StateTable* GetStateTableFromID(uint32_t InTableID) const;

			// getters
			EA::Allocator::IAllocator* GetStateMachineAllocator() const { return m_StateMachineAllocator; }

			/* get the shared instance of this manager */
			static StateMachineManager* GetInstance();

		private:

			EA::Allocator::IAllocator* m_StateMachineAllocator = nullptr;
			uint32_t m_AllocationNumber = 0;
			char m_Padding[0x14];
		};

		static_assert(sizeof(EARS::StateMachineSys::StateMachineManager) == 0x20, "EARS::StateMachineSys::StateMachineManager must equal 0x20");
	} // StateMachineSys
} // EARS
