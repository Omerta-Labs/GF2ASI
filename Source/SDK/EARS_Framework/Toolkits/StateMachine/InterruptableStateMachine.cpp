#include "InterruptableStateMachine.h"

#include "Addons/Hook.h"

// CPP
#include <assert.h>

EARS::Framework::InterruptableStateMachine::InterruptableStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
	: EARS::StateMachineSys::StateMachine(TableID, SMParams)
	, m_InterruptFlags(0)
	, m_InterruptsHandled(0)
	, m_InterruptsChildrenHandleCache(0)
	, m_bInterruptsCacheDirty(false)
{
}

EARS::Framework::InterruptableStateMachine::~InterruptableStateMachine()
{
	if (m_InterruptsChildrenHandleCache || m_InterruptsHandled)
	{
		SetParentInterruptDirtyFlag();
	}
}

void EARS::Framework::InterruptableStateMachine::SetParentInterruptDirtyFlag() const
{
	// NB: Dynamic_cast might not work here...
	// TODO: Validate casting
	if (EARS::Framework::InterruptableStateMachine* OwningSM = dynamic_cast<InterruptableStateMachine*>(GetParent()))
	{
		if (!OwningSM->m_bInterruptsCacheDirty)
		{
			OwningSM->SetInterruptDirtyFlag();
		}
	}
}

void EARS::Framework::InterruptableStateMachine::SetInterruptDirtyFlag()
{
	m_bInterruptsCacheDirty = true;
	SetParentInterruptDirtyFlag();
}
