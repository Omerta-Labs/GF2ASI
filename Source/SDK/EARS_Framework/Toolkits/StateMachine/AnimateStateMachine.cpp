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
