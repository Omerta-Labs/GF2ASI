#include "InterruptableStateMachine.h"

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

void EARS::Framework::InterruptableStateMachine::SetInterruptsHandled(uint32_t InterruptsHandled)
{
	m_InterruptsHandled |= InterruptsHandled;
	SetParentInterruptDirtyFlag();
}

void EARS::Framework::InterruptableStateMachine::ClearInterruptsHandled(uint32_t InterruptsHandled)
{
	m_InterruptsHandled &= ~InterruptsHandled;
	SetParentInterruptDirtyFlag();
}

void EARS::Framework::InterruptableStateMachine::InitInterruptsHandled(uint32_t InterruptsHandled)
{
	m_InterruptsHandled = InterruptsHandled;
	SetParentInterruptDirtyFlag();
}

void EARS::Framework::InterruptableStateMachine::ClearInterrupts()
{
	m_InterruptFlags = 0;
}

bool EARS::Framework::InterruptableStateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
{
	switch (TransID)
	{
	case TRANSID_TESTINTERRUPT:
	{
		return TestInterruptFlag(TransData->m_Data.m_IntegerVal);
	}
	case TRANSID_TESTANDCLEARINTERRUPT:
	{
		const uint32_t flag = TransData->m_Data.m_IntegerVal;
		const bool bResult = TestInterruptFlag(flag);
		if (bResult)
		{
			ClearInterruptFlag(flag);
		}
		return bResult;
	}
	default:
	{
		return StateMachine::CheckTransition(SimTime, FrameTime, TransID, TransData);
	}
	}
}

bool EARS::Framework::InterruptableStateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
{
	switch (MessageID)
	{
	case MESSAGE_SETINTERRUPTHANDLE:
	{
		SetInterruptsHandled(MsgData->GetIntData());
		return true;
	}
	case MESSAGE_CLEARINTERRUPTHANDLE:
	{
		ClearInterruptsHandled(MsgData->GetIntData());
		return true;
	}
	case MESSAGE_INITINTERRUPTHANDLE:
	{
		InitInterruptsHandled(MsgData->GetIntData());
		return true;
	}
	case MESSAGE_SENDINTERRUPT:
	{
		SendInterrupt(MsgData->GetIntData());
		return true;
	}
	default:
	{
		return StateMachine::HandleStateMessage(SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}
	}
}

EARS::StateMachineSys::StateMachine* EARS::Framework::InterruptableStateMachine::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
{
	using namespace EARS::StateMachineSys;

	StateMachine* Result = EARS::StateMachineSys::StateMachine::Update(SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
	if (Result)
	{
		ClearInterrupts();
	}

	return Result;
}

uint32_t EARS::Framework::InterruptableStateMachine::RecalculateInterruptsHandledBySM()
{
	using namespace EARS::StateMachineSys;

	if (m_bInterruptsCacheDirty)
	{
		m_bInterruptsCacheDirty = false;
		m_InterruptsChildrenHandleCache = 0;

		StateMachine::ChildList List = GetChildList();
		for (StateMachine* Child : List)
		{
			// TODO: Validate this
			if (InterruptableStateMachine* InterruptChild = dynamic_cast<InterruptableStateMachine*>(Child))
			{
				m_InterruptsChildrenHandleCache |= InterruptChild->RecalculateInterruptsHandledBySM();
			}
		}
	}

	return m_InterruptsChildrenHandleCache | m_InterruptsHandled;
}

bool EARS::Framework::InterruptableStateMachine::TestInterruptFlag(uint32_t Interrupt) const
{
	return (Interrupt & m_InterruptFlags) != 0;
}

void EARS::Framework::InterruptableStateMachine::ClearInterruptFlag(uint32_t Interrupt)
{
	m_InterruptFlags &= ~Interrupt;
}

void EARS::Framework::InterruptableStateMachine::SendInterrupt(uint32_t flagID)
{
	EARS::StateMachineSys::StateMachineTree* pTree = GetTree();
	if (!pTree)
	{
		return;
	}

	InterruptableStateMachine* pRoot = static_cast<InterruptableStateMachine*>(pTree->GetRoot());
	if (!pRoot)
	{
		return;
	}

	pRoot->RecalculateInterruptsHandledBySM();

	if (InterruptableStateMachine* pHandler = pRoot->FindInterruptHandler(flagID))
	{
		pHandler->m_InterruptFlags |= flagID;
	}
}

EARS::Framework::InterruptableStateMachine* EARS::Framework::InterruptableStateMachine::FindInterruptHandler(uint32_t interrupt)
{
	if (m_InterruptsChildrenHandleCache & interrupt)
	{
		StateMachine::ChildList List = GetChildList();
		for (StateMachine* Child : List)
		{
			// TODO: Validate this
			if (InterruptableStateMachine* InterruptChild = dynamic_cast<InterruptableStateMachine*>(Child))
			{
				if (InterruptableStateMachine* pResult = InterruptChild->FindInterruptHandler(interrupt))
				{
					return pResult;
				}
			}
		}

		return nullptr;
	}
	else if ((m_InterruptsHandled & interrupt) != 0)
	{
		return this;
	}

	return nullptr;
}

void EARS::Framework::InterruptableStateMachine::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
{
	using namespace EARS::StateMachineSys;

	SetInterruptDirtyFlag();
	StateMachine::AddChild(SimTime, FrameTime, Child);
}
