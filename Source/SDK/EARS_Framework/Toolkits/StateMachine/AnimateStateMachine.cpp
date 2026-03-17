#include "AnimateStateMachine.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Framework/Core/Animated/Animated.h"

EARS::Framework::AnimateStateMachine::AnimateStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
	: EARS::Framework::InterruptableStateMachine(TableID, SMParams)
{
	EARS::Framework::AnimateStateMachineParams* AnimateParams = static_cast<EARS::Framework::AnimateStateMachineParams*>(SMParams);
	m_AnimatedEntity = AnimateParams->GetOwner();
}

EARS::Framework::AnimateStateMachine::~AnimateStateMachine()
{
	// m_AnimatedEntity is cleared automatically via destructor
}

void EARS::Framework::AnimateStateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
{
	// source: AnimateStateMachine::SpawnChild (PC ASM sub_4AC7E0, migrated from PlayerSM stub).
	MemUtils::CallClassMethod<void, AnimateStateMachine*, uint32_t, float, uint32_t>(0x4AC7E0, this, SimTime, FrameTime, TableID);
}
