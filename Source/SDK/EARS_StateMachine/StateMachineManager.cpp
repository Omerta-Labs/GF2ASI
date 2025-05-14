#include "StateMachineManager.h"

#include "Addons/Hook.h"

namespace EARS
{
	namespace StateMachineSys
	{
		StateTable* StateMachineManager::GetStateTableFromID(uint32_t InTableID) const
		{
			return MemUtils::CallClassMethod<StateTable*, const StateMachineManager*, uint32_t>(0x0621530, this, InTableID);
		}

		StateMachineManager* StateMachineManager::GetInstance()
		{
			return *(StateMachineManager**)0x12234AC;
		}
	} // StateMachineSys
} // EARS