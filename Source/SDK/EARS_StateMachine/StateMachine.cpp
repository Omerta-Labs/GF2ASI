#include "SDK/EARS_StateMachine/StateMachine.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Common/IAllocator.h"
#include "SDK/EARS_StateMachine/StateMachineManager.h"

// C++
#include <assert.h>

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
	//if (MessageID == 1)
	//{
	//	void* Value = MsgData->m_Data.m_PointerVal;
	//	AddChildrenMessageData* ChildMsgData = reinterpret_cast<AddChildrenMessageData*>(Value);

	//	for (uint16_t ChildIdx = 0; ChildIdx < ChildMsgData->m_NumStateMachines; ChildIdx++)
	//	{		
	//		SpawnChild(SimTime, FrameTime, ChildMsgData->m_StateMachineIDs[ChildIdx]);
	//	}
	//}
	//using namespace EARS::StateMachineSys;

	return MemUtils::CallClassMethod<bool, StateMachine*, uint32_t, float, uint32_t, uint32_t, State::StateMessageData*>(0x624C20, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
}

bool EARS::StateMachineSys::StateMachine::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData)
{
	using namespace EARS::StateMachineSys;

	return MemUtils::CallClassMethod<bool, StateMachine*, uint32_t, float, uint32_t, void*>(0x624B00, this, SimTime, FrameTime, TransID, TransData);
}

void EARS::StateMachineSys::StateMachine::InitialiseChild(StateMachine* ChildMachine)
{
	// nothing in default implementation
	int z = 0;
}

void EARS::StateMachineSys::StateMachine::Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel)
{
	MemUtils::CallClassMethod<void, StateMachine*, uint32_t, float, uint32_t, uint32_t&, uint32_t>(0x624870, this, SimTime, FrameTime, CurFlags, TerminateLevel, EvalLevel);
}

void EARS::StateMachineSys::StateMachine::AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child)
{
	using namespace EARS::StateMachineSys;

	MemUtils::CallClassMethod<void, StateMachine*, uint32_t, float, StateMachine*>(0x0624DC0, this, SimTime, FrameTime, Child);
}

void EARS::StateMachineSys::StateMachine::RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine)
{
	using namespace EARS::StateMachineSys;

	MemUtils::CallClassMethod<void, StateMachine*, StateMachine*>(0x624E00, this, ChildMachine);
}

void EARS::StateMachineSys::StateMachine::SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID)
{
	MemUtils::CallClassMethod<void, StateMachine*, uint32_t, float, uint32_t>(0x624B60, this, SimTime, FrameTime, TableID);
}

void EARS::StateMachineSys::StateMachine::ReadInitDataFromSnapShot(void* SnapshotData)
{
	// TODO: Figure out once its properly working
}

void EARS::StateMachineSys::StateMachine::WriteInitDataToSnapShot(void* SnapshotData)
{
	// TODO: Figure out once its properly working
}

void EARS::StateMachineSys::StateMachine::SetExternalTransitions(TransitionList* ExternalTransitions)
{
	m_ExternalTransitions = ExternalTransitions;
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
