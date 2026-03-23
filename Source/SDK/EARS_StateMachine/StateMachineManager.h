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
		struct StateMachineParams;
		class StateMachine;

		class StateMachineManager : public Singleton<StateMachineManager>
		{
		public:

			virtual ~StateMachineManager() { /* implemented in engine */ }

			/* Fetch State Table using Given ID */
			StateTable* GetStateTableFromID(uint32_t InTableID) const;

			/* Create a new StateMachine instance from a table ID; params may be nullptr */
			StateMachine* CreateStateMachineFromTableID(uint32_t StateTableID, StateMachineParams* Params);

			// getters
			EA::Allocator::IAllocator* GetStateMachineAllocator() const { return m_StateMachineAllocator; }

			/* get the shared instance of this manager */
			static StateMachineManager* GetInstance();

		private:

			// Internally create a new State Machine by resolving their factory and returning the result.
			StateMachine* CreateStateMachine(uint32_t InSMTableID, uint32_t StateTableID, StateMachineParams* Params);

			/**
			 * Looks up a factory function pointer from the internal factory map by class ID.
			 *
			 * The original PC function at 0x621610 is __userpurge (this in EAX, tree root
			 * in ESI) which cannot be called via standard __thiscall.  This naked thunk
			 * reproduces the original code with 'this' sourced from ECX instead, allowing
			 * it to be called as a normal __thiscall member function.
			 */
			typedef StateMachine* (*FactoryFunc)(uint32_t, StateMachineParams*);
			FactoryFunc GetFactoryFromID(uint32_t ClassID) const;

			EA::Allocator::IAllocator* m_StateMachineAllocator = nullptr;
			uint32_t m_AllocationNumber = 0;
			char m_Padding[0x14];
		};

		static_assert(sizeof(EARS::StateMachineSys::StateMachineManager) == 0x20, "EARS::StateMachineSys::StateMachineManager must equal 0x20");
	} // StateMachineSys
} // EARS
