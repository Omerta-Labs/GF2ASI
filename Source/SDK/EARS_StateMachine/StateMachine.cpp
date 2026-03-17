#include "SDK/EARS_StateMachine/StateMachine.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Common/IAllocator.h"
#include "SDK/EARS_StateMachine/StateMachineManager.h"

// C++
#include <assert.h>
#include <stdio.h>
#include <string.h>

EARS::StateMachineSys::TransitionList::TransitionList()
	: m_Count(0)
	, m_bIsActive(false)
	, m_bIsMaster(false)
{
	for (uint32_t i = 0; i < 0x20; i++)
	{
		m_bUsed[i] = 0;
	}
}

void EARS::StateMachineSys::TransitionList::Add(uint32_t StateTableID, uint32_t TransID, uint32_t EvalLevel)
{
	for (uint32_t i = 0; i < 32; i++)
	{
		if (m_bUsed[i] == false)
		{
			m_bUsed[i] = true;
			m_StateTable[i] = StateTableID;
			m_Transitions[i] = TransID;
			m_EvalTable[i] = EvalLevel;
			m_Count++;
			break;
		}
	}
}

bool EARS::StateMachineSys::TransitionList::Remove(uint32_t StateTableID, uint32_t TransID, uint32_t EvalLevel)
{
	bool bRemoved = false;
	for (uint32_t i = 0; i < 32; i++)
	{
		if (m_bUsed[i] && StateTableID == m_StateTable[i] && TransID == m_Transitions[i] && EvalLevel == m_EvalTable[i])
		{
			m_bUsed[i] = false;
			m_Count--;
			bRemoved = true;
			break;
		}
	}

	return bRemoved;
}

EARS::StateMachineSys::StateMachine::StateMachine(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams)
	: m_StateTableID(TableID)
	, m_CurStateIdx(0)
	, m_EvalFlags(0)
	, m_Tree(nullptr)
	, m_Parent(nullptr)
	, m_Next(nullptr)
	, m_ChildTail(nullptr)
	, m_ChildHead(nullptr)
	, m_TimeInCurrentState(0.0f)
	, m_AllocNumber(0)
{
	StateMachineManager* StateMachineMgr = StateMachineManager::GetInstance();
	StateTable* StateTable = StateMachineMgr->GetStateTableFromID(m_StateTableID);
	assert(StateTable);

	StateTable->IncRefCount();
	m_States = StateTable->GetStates();

	SetExternalTransitions(SmParams->m_Transitions);
}

EARS::StateMachineSys::StateMachine::~StateMachine()
{
	StateMachineManager* StateMachineMgr = StateMachineManager::GetInstance();
	StateTable* StateTable = StateMachineMgr->GetStateTableFromID(m_StateTableID);
	assert(StateTable);

	StateTable->DecRefCount();
}

bool EARS::StateMachineSys::StateMachine::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, State::StateMessageData* MsgData)
{
	// Source: PC ASM 0x624C20 — reconstructed from x86 body.
	// Handles two built-in messages:
	//   MessageID 1 — spawn children listed in AddChildrenMessageData (AddChildren message)
	//   MessageID 2 — remove this state machine from its tree (self-destruct signal)
	// All other MessageIDs return false (unhandled).

	switch (MessageID)
	{
	case State::StateMessageID::MESSAGE_ADD_CHILDREN:
	{
		// Spawn children: MsgData->m_Data.m_PointerVal → AddChildrenMessageData*
		AddChildrenMessageData* data = reinterpret_cast<AddChildrenMessageData*>(MsgData->m_Data.m_PointerVal);
		if ((data->m_Flags & 1) != 0)
		{
			SetEvaluateTransitions(true);
		}
		else
		{
			SetEvaluateTransitions(false);
		}

		for (uint16_t i = 0; i < data->m_NumStateMachines; i++)
		{
			SpawnChild(SimTime, FrameTime, data->m_StateMachineIDs[i]);
		}

		return true;
	}
	case State::StateMessageID::MESSAGE_TERMINATE:
	{
		// Remove from tree if we are not already terminating and have a tree
		if (!(m_EvalFlags & 2) && m_Tree)
		{
			m_Tree->Remove(this);
		}

		return true;
	}
	default:
	{
		return false;
	}
	}
}

bool EARS::StateMachineSys::StateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, Transition::TransitionData* TransData)
{
	// Source: PC ASM 0x624B00 — reconstructed from x86 body.
	// Switch on TransID (built-in transition conditions):
	//   1 — HasNoChildren:          true when m_ChildHead == null
	//   2 — TimeInStateGreaterThan: true when m_TimeInCurrentState > TransData->float_at_4
	//   3 — TimeInStateAlways:      true when m_TimeInCurrentState > global constant (0xE2B05C)
	//   4 — Always:                 unconditionally true
	//   default:                    false

	using StateMachineTransID = Transition::StateMachineTransID;

	switch (TransID)
	{
	case StateMachineTransID::TRANSID_NOCHILDREN:
	{
		return (HasChildren() == false);
	}
	case StateMachineTransID::TRANSID_TIMEOUT:
	{
		assert(TransData->m_TypeID == StateMachineTypeID::c_SMFloatDataID);
		return GetTimeInCurState() > TransData->m_Data.m_FloatVal;
	}
	case StateMachineTransID::TRANSID_NEXTFRAME:
	{
		// PC ASM: comiss xmm0, ds:dword_E2B05C  (address 0xE2B05C, dd 0 in .data section)
		// dword_E2B05C is a mutable global float initialised to 0.0f; fires when
		// m_TimeInCurrentState > 0.0f (i.e. at least one frame has elapsed in this state).
		// NOTE: if dword_E2B05C is written at runtime the threshold would change, but
		// no write sites have been found — treating as the constant 0.0f is safe for now.
		return GetTimeInCurState() > 0.0f;
	}
	case StateMachineTransID::TRANSID_DEFAULT:
	{
		return true;
	}
	default:
	{
		return false;
	}
	}
}

void EARS::StateMachineSys::StateMachine::InitialiseChild(StateMachine* ChildMachine)
{
	// nothing in default implementation
	int z = 0;
}

EARS::StateMachineSys::StateMachine* EARS::StateMachineSys::StateMachine::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
{
	// Source: PC ASM 0x624870 — reconstructed from x86 body.
	//
	//
	// EvalFlags bits used here:
	//   bit 0 (1): currently updating (set on entry, cleared on exit)
	//   bit 1 (2): self-termination requested (triggers delete + parent notify on exit)
	//   bit 2 (4): override children — force transition processing even when children exist
	//   bit 3 (8): child-termination signal received from child (triggers re-evaluation)
	//
	// External TransitionList helpers (sub_625390 / sub_625340):
	//   sub_625390: search and consume a pending transition entry in an active slave list
	//   sub_625340: record a fired transition into a master list for slave SMs to consume
	//   Both are inlined below from their PC ASM bodies.



	m_EvalFlags |= 1;   // mark as actively updating

	State* curState = m_States[m_CurStateIdx];
	if (!curState)
		goto cleanup;

	// --- Update messages (skipped if self-termination is already pending) ---
	if (!(m_EvalFlags & 8))
	{
		State::StateMessage** msgPtr = curState->m_UpdateMessages;
		for (State::StateMessage* msg = *msgPtr; msg; msg = *++msgPtr)
		{
			HandleStateMessage(SimTime, FrameTime, CurFlags, msg->m_MessageID, msg->m_StateMessageData);
		}
	}

	{
		// --- Transition loop ---
		Transition** transSlot = reinterpret_cast<Transition**>(curState->m_Transitions);
		Transition* transition = *transSlot;
		if (!transition)
			goto cleanup;

		int loopCount = 0;
		while (true)
		{
			// Abort if termination was requested mid-loop
			if (m_EvalFlags & 2)
				goto cleanup;

			// sub_624720: can we process transitions?
			// false when: has a tree AND has children AND EvalFlags&4 is clear
			// (children drive their own transitions; parent waits unless overridden)
			if (m_Tree && m_ChildHead && !(m_EvalFlags & 4))
				goto cleanup;

			// --- Evaluate current transition ---
			bool fired;
			if (transition->m_Flags & 1)
			{
				// CheckTransition path
				if (m_ExternalTransitions && m_ExternalTransitions->IsActive() && !m_ExternalTransitions->IsMaster())
				{
					// sub_625390: check-and-consume a matching entry from the slave TransitionList.
					// Returns true if a matching (stateTableID, transID, evalLevel) entry was found
					// and removed from the list, false if not found.
					// The master SM previously recorded this transition via sub_625340 (TransitionList::Add).
					fired = m_ExternalTransitions->Remove(m_StateTableID, transition->m_TransIDOrFlags, EvalLevel);
				}
				else
				{
					fired = CheckTransition(SimTime, FrameTime, transition->m_TransIDOrFlags, transition->m_TransData);
				}
			}
			else
			{
				// Flag-mask path: transition fires when all required flags are present in CurFlags
				fired = ((transition->m_TransIDOrFlags & CurFlags) == transition->m_TransIDOrFlags);
			}

			if (!fired)
			{
				// Advance to next transition in the null-terminated array
				++transSlot;
				transition = *transSlot;
				if (!transition)
				{
					break;  // exhausted all transitions → fall to cleanup
				}

				continue;
			}

			// --- Transition fired ---

			// sub_625340: if we are the master external list, record the firing for slaves
			if (m_ExternalTransitions && m_ExternalTransitions->IsActive() && m_ExternalTransitions->IsMaster())
			{
				// Find first unused slot and record (stateTableID, transID, evalLevel)
				m_ExternalTransitions->Add(m_StateTableID, transition->m_TransIDOrFlags, EvalLevel);
			}

			// Exit messages for the current state (no terminate check mid-loop)
			{
				State::StateMessage** exitPtr = curState->m_ExitMessages;
				for (State::StateMessage* msg = *exitPtr; msg; msg = *++exitPtr)
				{
					HandleStateMessage(SimTime, FrameTime, CurFlags, msg->m_MessageID, msg->m_StateMessageData);
				}
			}

			// Remove and delete all children before changing state
			TerminateChildren();

			// Execute the state change
			TerminateLevel      = EvalLevel;
			m_CurStateIdx       = transition->m_TargetStateIdx;
			m_TimeInCurrentState = 0.0f;
			curState            = m_States[m_CurStateIdx];

			// Enter messages for the new state (terminate check after each)
			{
				State::StateMessage** enterPtr = curState->m_EnterMessages;
				for (State::StateMessage* msg = *enterPtr; msg; msg = *++enterPtr)
				{
					HandleStateMessage(SimTime, FrameTime, CurFlags, msg->m_MessageID, msg->m_StateMessageData);
					if (m_EvalFlags & 2)
					{
						goto cleanup;   // termination triggered during enter processing
					}
				}
			}

			// Safety limit: abort after 25 chained transitions in one Update
			if (++loopCount > 25)
			{
				goto cleanup;
			}

			// Restart the loop with the new state's transition table
			transSlot  = reinterpret_cast<Transition**>(curState->m_Transitions);
			transition = *transSlot;
			if (!transition)
			{
				break;  // new state has no transitions → fall to cleanup
			}
		}
	}

cleanup:
	// Clear "updating" (bit 0) and "child-terminate" (bit 3) flags
	m_EvalFlags      &= ~(1u | 8u);
	m_TimeInCurrentState += FrameTime;

	if (m_EvalFlags & 2)
	{
		// Self-termination: save parent before we delete ourselves
		StateMachine* parent = m_Parent;
		delete this;    // calls virtual ~StateMachine → scalar deleting destructor

		if (parent)
		{
			parent->m_EvalFlags |= 8;   // signal child-termination to parent
			TerminateLevel = EvalLevel;
			parent->Update(SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel + 1);
		}
		return nullptr;
	}

	return this;
}

void EARS::StateMachineSys::StateMachine::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
{
	using namespace EARS::StateMachineSys;

	assert(Child);

	// link up with tree
	if (m_Tree)
	{
		Child->m_Tree = m_Tree;
	}

	// attach to list
	if (m_ChildTail)
	{
		m_ChildTail->m_Next = Child;
	}
	else
	{
		m_ChildHead = Child;
	}

	// lnik up in hierarchy
	m_ChildTail = Child;
	Child->m_Parent = this;

	// begin state
	Child->InitializeState(SimTime, FrameTime);
}

void EARS::StateMachineSys::StateMachine::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
{
	// source: ?RemoveChild@StateMachine@StateMachineSys@EARS@@UAAXPAV123@@Z
	// Reconstructed from Xbox 360 PPC disassembly (PDB [0003:00F38FF0], 0x198 bytes).
	// Unlinks ChildMachine from the singly-linked child list (head/tail/next),
	// then clears its m_Tree and m_Next pointers. Does NOT clear m_Parent.
	assert(ChildMachine != nullptr); // "pChild", statemachine.cpp line 553

	if (ChildMachine == m_ChildHead && ChildMachine == m_ChildTail)
	{
		// only child
		m_ChildHead = nullptr;
		m_ChildTail = nullptr;
	}
	else if (ChildMachine == m_ChildHead)
	{
		// removing the head
		m_ChildHead = m_ChildHead->m_Next;
	}
	else
	{
		// scan for the node
		StateMachine* prev = m_ChildHead;
		StateMachine* curr = m_ChildHead->m_Next;
		while (curr != nullptr)
		{
			if (curr == ChildMachine)
			{
				if (curr == m_ChildTail)
				{
					prev->m_Next = nullptr;
					m_ChildTail  = prev;
				}
				else
				{
					prev->m_Next = curr->m_Next;
				}
				break;
			}
			prev = curr;
			curr = curr->m_Next;
		}
	}

	ChildMachine->m_Tree = nullptr;
	ChildMachine->m_Next = nullptr;
}

void EARS::StateMachineSys::StateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
{
	//// source: ?SpawnChild@StateMachine@StateMachineSys@EARS@@UAAXIIM@Z
	//// Reconstructed from Xbox 360 PPC disassembly (PDB [0003:00F387F0], 0xAC bytes).
	//// Creates a new child state machine from the given table ID via the manager,
	//// propagates our external transitions, calls the virtual InitialiseChild hook,
	//// then links the child into the tree with AddChild.
	StateMachineManager* mgr = StateMachineManager::GetInstance();
	StateMachine* pChild = mgr->CreateStateMachineFromTableID(TableID, nullptr);
	pChild->SetExternalTransitions(m_ExternalTransitions);
	InitialiseChild(pChild);
	AddChild(SimTime, FrameTime, pChild);
}

const EARS::StateMachineSys::StateMachineSnapshot* EARS::StateMachineSys::StateMachine::ReadInitDataFromSnapShot(const StateMachineSnapshot* pSnap)
{
	// source: ReadInitDataFromSnapShot virtual (base class body decoded from 360 ASM).
	// The base class implementation is a no-op data-wise; it simply advances the
	// snapshot cursor past this node's 0x10-byte slot and returns the next pointer.
	// Derived classes override to read m_CurStateIdx / m_EvalFlags before calling super.
	return pSnap + 1;
}

EARS::StateMachineSys::StateMachineSnapshot* EARS::StateMachineSys::StateMachine::WriteInitDataToSnapShot(StateMachineSnapshot* pSnap)
{
	// source: WriteInitDataToSnapshot virtual (base class body decoded from 360 ASM).
	// Clears the padding/numChildren field at offset 0x0E, advances the cursor by
	// one node (0x10 bytes) and returns the next write pointer.
	// Derived classes override to write m_CurStateIdx / m_EvalFlags before calling super.
	pSnap->m_Pad = 0;
	return pSnap + 1;
}

const char* EARS::StateMachineSys::StateMachine::GetCurrentStateName() const
{
	// source: ?GetCurrentStateName@StateMachine@StateMachineSys@EARS@@QBAPBDXZ
	// Returns the name of the current state, or nullptr if no state table is loaded.
	if (const State* curState = m_States[m_CurStateIdx])
	{
		return curState->m_Name;
	}

	return nullptr;
}

const char* EARS::StateMachineSys::StateMachine::GetTableName() const
{
	// source: ?GetTableName@StateMachine@StateMachineSys@EARS@@QBAPBDXZ
	// Looks up the state table by ID via StateMachineManager and returns its name.
	StateMachineManager* mgr = StateMachineManager::GetInstance();
	if (StateTable* table = mgr->GetStateTableFromID(m_StateTableID))
	{
		return table->GetName();
	}

	return nullptr;
}

unsigned int EARS::StateMachineSys::StateMachine::PrintStateMachine(char* buf, unsigned int bufSize, unsigned int level) const
{
	// source: ?PrintStateMachine@StateMachine@StateMachineSys@EARS@@QBAIPADII@Z
	// Reconstructed from Xbox 360 PPC disassembly.
	// Appends a tree-formatted line to buf for this state machine and recurses into children.
	// Format per node: ".  " × level + "<TableName> <StateName> <TimeInState>\n"
	// Returns the remaining buffer capacity (bufSize decremented by each formatted line).

	// Build indentation: append ".  " for each level
	for (unsigned int i = 0; i < level; i++)
	{
		strcat(buf, ".  ");
	}

	const float timeInCurState  = GetTimeInCurState();
	const char* stateName = GetCurrentStateName();
	const char* tableName = GetTableName();

	// Format the state line into a local buffer (~512 bytes, matching the 0x270-byte local in the original)
	char lineBuf[0x270];
	int lineSize = sprintf(lineBuf, "%s %s %.1f\n", tableName, stateName, timeInCurState);

	// Original assert: "lineSize < bufSize"  (statemachine.cpp line 618)
	assert(lineSize < (int)bufSize);

	strcat(buf, lineBuf);
	bufSize -= (unsigned int)lineSize;

	// Recurse into children
	ChildList children = GetChildList();
	for (ChildList::const_iterator it = children.begin(); it != children.end(); ++it)
	{
		bufSize = (*it)->PrintStateMachine(buf, bufSize, level + 1);
	}

	return bufSize;
}

void EARS::StateMachineSys::StateMachine::SetExternalTransitions(TransitionList* ExternalTransitions)
{
	m_ExternalTransitions = ExternalTransitions;
}

void EARS::StateMachineSys::StateMachine::InitializeState(uint32_t simTime, float frameTime)
{
	assert(m_States);

	State* CurState = *m_States;
	assert(CurState);

	m_CurStateIdx = 0;
	State::StateMessage** MsgPtr = CurState->m_EnterMessages;
	State::StateMessage* Msg = *MsgPtr;
	while (Msg)
	{
		HandleStateMessage(simTime, frameTime, 0, Msg->m_MessageID, Msg->m_StateMessageData);

		// next...
		MsgPtr++;
		Msg = *MsgPtr;
	}
}

void EARS::StateMachineSys::StateMachine::SetEvaluateTransitions(bool bCheck)
{
	// bit 0 of m_Flags controls EvalFlags bit 2 (used by CanProcessTransitions check)
	if (bCheck)
	{
		m_EvalFlags |= 4;
	}
	else
	{
		m_EvalFlags &= ~4u;
	}
}

void EARS::StateMachineSys::StateMachine::TerminateChildren()
{
	// source: sub_624D20 (PC address 0x624D20) — reconstructed from x86 body.
	// Iterates m_ChildHead; for each child:
	//   1. Recurse into the child's subtree first (depth-first).
	//   2. Remove the child from this SM's list via RemoveChild.
	//   3. If the child is mid-Update (EvalFlags & 1): mark it for deferred termination (EvalFlags |= 2).
	//      Otherwise: delete it immediately (calls virtual destructor + frees memory).
	// After RemoveChild, m_ChildHead advances to the next child automatically.
	while (StateMachine* child = m_ChildHead)
	{
		child->TerminateChildren();   // recurse depth-first
		RemoveChild(child);
		child->Destroy();
	}
}

void EARS::StateMachineSys::StateMachine::Destroy()
{
	if (IsBusy())
	{
		m_EvalFlags |= 2;    // currently updating — defer deletion
	}
	else
	{
		delete this;               // safe to delete now
	}
}

void* EARS::StateMachineSys::StateMachine::operator new(size_t size)
{
	using namespace EARS::StateMachineSys;

	StateMachineManager* StateMachineMgr = StateMachineManager::GetInstance();

	// fetch allocator and try to allocate
	EA::Allocator::IAllocator* SMAllocator = StateMachineMgr->GetStateMachineAllocator();
	assert(SMAllocator);

	EA::TagValuePair Tag;
	return SMAllocator->Alloc(size, Tag);
}

void EARS::StateMachineSys::StateMachine::operator delete(void* p)
{
	using namespace EARS::StateMachineSys;

	StateMachineManager* StateMachineMgr = StateMachineManager::GetInstance();

	// fetch allocator and to free the memory
	EA::Allocator::IAllocator* SMAllocator = StateMachineMgr->GetStateMachineAllocator();
	assert(SMAllocator);

	SMAllocator->Free(p, 0);
}

void EARS::StateMachineSys::StateMachineTree::Remove(StateMachine* pSM)
{
	// source: ?Remove@StateMachineTree@StateMachineSys@EARS@@QAAXPAVStateMachine@23@@Z
	// PC address: 0x624F30
	MemUtils::CallClassMethod<void, StateMachineTree*, StateMachine*>(0x624F30, this, pSM);
}
