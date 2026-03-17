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

		StateMachine* StateMachineManager::CreateStateMachineFromTableID(uint32_t TableID, StateMachineParams* Params)
		{
			// TODO-CLAUDE: Not yet implemented — needs full 360 ASM decode of
			// ?CreateStateMachineFromTableID@StateMachineManager@StateMachineSys@EARS@@
			// SpawnChild calls this, so until it is filled in SpawnChild will malfunction.
			return MemUtils::CallClassMethod<StateMachine*, StateMachineManager*, uint32_t, StateMachineParams*>(0x6215A0, this, TableID, Params);
		}

		StateMachineManager* StateMachineManager::GetInstance()
		{
			return *(StateMachineManager**)0x12234AC;
		}
	} // StateMachineSys
} // EARS