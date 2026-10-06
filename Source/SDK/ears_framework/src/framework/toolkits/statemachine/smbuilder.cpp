#include "smbuilder.h"

#include "Platform/MemUtils.h"

// SDK
#include "allocator/iallocator.h"
#include "ears_statemachine/statemachinemanager.h"

//
// SMBuilderState
//
// The engine allocates every SMBuilderState out of the owning builder's allocator, so we never
// construct one.  The three virtual bodies below exist only to declare the slots; a call through an
// engine-owned pointer dispatches into the engine's own vtable instead.
//

EARS::Framework::SMBuilderState::~SMBuilderState()
{
	// 0x04ACB20 is the deleting destructor thunk that occupies vtable slot 0.  A flag of 0 means
	// destroy the members without releasing the allocation, which is what ~SMBuilder wants: it
	// frees the storage itself afterwards.
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, int>(0x04ACB20, this, 0);
}

uint32_t EARS::Framework::SMBuilderState::GetSize() const
{
	return MemUtils::CallClassMethod<uint32_t, const EARS::Framework::SMBuilderState*>(0x04AD4A0, this);
}

EARS::StateMachineSys::State* EARS::Framework::SMBuilderState::Compile(void* InBuffer, uint32_t& OutBytesWritten) const
{
	using namespace EARS::Framework;
	using namespace EARS::StateMachineSys;
	return MemUtils::CallClassMethod<State*, const SMBuilderState*, void*, uint32_t*>(0x04AD780, this, InBuffer, &OutBytesWritten);
}

void EARS::Framework::SMBuilderState::AddChild(const char* ChildName, bool bEvalParentTrans)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, bool>(0x04ACF10, this, ChildName, bEvalParentTrans);
}

void EARS::Framework::SMBuilderState::AddTransition(const char* DestStateName, uint32_t MsgID)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, uint32_t>(0x04ACC70, this, DestStateName, MsgID);
}

void EARS::Framework::SMBuilderState::AddTransition(const char* DestStateName, uint32_t MsgID, const void* InData, uint32_t TypeID, uint32_t DataSize, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, uint32_t, const void*, uint32_t, uint32_t, uint32_t>(0x04ACCC0, this, DestStateName, MsgID, InData, TypeID, DataSize, Alignment);
}

void EARS::Framework::SMBuilderState::AddIntTransition(const char* DestStateName, uint32_t MsgID, uint32_t Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, uint32_t, uint32_t, uint32_t>(0x04ACD20, this, DestStateName, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddFloatTransition(const char* DestStateName, uint32_t MsgID, float Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, uint32_t, float, uint32_t>(0x04ACD80, this, DestStateName, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddEnterAnim(int AnimID, bool bBlend, bool bForceAnim, bool bIgnoreGameMovementBlend, float FrameRateScale)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, int, bool, bool, bool, float>(0x04ACE70, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale);
}

void EARS::Framework::SMBuilderState::AddEnterMessage(uint32_t MsgID)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t>(0x04ACF70, this, MsgID);
}

void EARS::Framework::SMBuilderState::AddExitMessage(uint32_t MsgID)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t>(0x04AD070, this, MsgID);
}

void EARS::Framework::SMBuilderState::AddUpdateMessage(uint32_t MsgID)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t>(0x04ACFF0, this, MsgID);
}

//
// The payload-carrying message forms.  The PC build strips these symbols; each one was identified
// by which of the three message lists it appends to and by the EARS::StateMachineSys::
// StateMachineTypeID it stamps into the message (c_SMIntDataID or c_SMFloatDataID).
//

void EARS::Framework::SMBuilderState::AddEnterMessage(uint32_t MsgID, uint32_t TypeID, uint32_t DataSize, const void* InData, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, uint32_t, uint32_t, const void*, uint32_t>(0x04ACFB0, this, MsgID, TypeID, DataSize, InData, Alignment);
}

void EARS::Framework::SMBuilderState::AddUpdateMessage(uint32_t MsgID, uint32_t TypeID, uint32_t DataSize, const void* InData, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, uint32_t, uint32_t, const void*, uint32_t>(0x04AD030, this, MsgID, TypeID, DataSize, InData, Alignment);
}

void EARS::Framework::SMBuilderState::AddExitMessage(uint32_t MsgID, uint32_t TypeID, uint32_t DataSize, const void* InData, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, uint32_t, uint32_t, const void*, uint32_t>(0x04AD0B0, this, MsgID, TypeID, DataSize, InData, Alignment);
}

void EARS::Framework::SMBuilderState::AddIntEnterMessage(uint32_t MsgID, uint32_t Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, uint32_t, uint32_t>(0x04AD0F0, this, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddIntUpdateMessage(uint32_t MsgID, uint32_t Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, uint32_t, uint32_t>(0x04AD130, this, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddIntExitMessage(uint32_t MsgID, uint32_t Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, uint32_t, uint32_t>(0x04AD170, this, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddFloatEnterMessage(uint32_t MsgID, float Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, float, uint32_t>(0x04AD1B0, this, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddFloatUpdateMessage(uint32_t MsgID, float Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, float, uint32_t>(0x04AD200, this, MsgID, Data, Alignment);
}

void EARS::Framework::SMBuilderState::AddFloatExitMessage(uint32_t MsgID, float Data, uint32_t Alignment)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, uint32_t, float, uint32_t>(0x04AD240, this, MsgID, Data, Alignment);
}

//
// SMBuilder
//

EARS::Framework::SMBuilder::SMBuilder(const char* InTableName, EA::Allocator::IAllocator* InAllocator)
	: m_Allocator(InAllocator)
	, m_States(InAllocator, EA::TagValuePair(0, 0))
	, m_Name(InTableName)
	, m_StateMachineClassID(0)
{
	m_Allocator->AddRef();
	m_StateTableID = StateMachineSys::StateMachineManager::GetStateTableIDFromName(m_Name);
}

EARS::Framework::SMBuilder::~SMBuilder()
{
	for (uint32_t StateIdx = 0; StateIdx < m_States.size(); StateIdx++)
	{
		EARS::Framework::SMBuilderState* State = m_States[StateIdx];
		State->~SMBuilderState();
		m_Allocator->Free(State, 0);
	}

	m_Allocator->Release();
	m_States.clear();
}

EARS::Framework::SMBuilderState* EARS::Framework::SMBuilder::AddState(const char* StateName, int StateEnum)
{
	using namespace EARS::Framework;
	return MemUtils::CallClassMethod<SMBuilderState*, SMBuilder*, const char*, int>(0x04ADAE0, this, StateName, StateEnum);
}

uint32_t EARS::Framework::SMBuilder::GetSize() const
{
	using namespace EARS::Framework;
	return MemUtils::CallClassMethod<uint32_t, const SMBuilder*>(0x04ADB70, this);
}

EARS::StateMachineSys::State** EARS::Framework::SMBuilder::Compile(void* InBuffer)
{
	using namespace EARS::Framework;
	using namespace EARS::StateMachineSys;
	return MemUtils::CallClassMethod<State**, SMBuilder*, void*>(0x04ADC20, this, InBuffer);
}

void EARS::Framework::SMBuilder::CompileAndRegister(uint32_t ClassID, EARS::StateMachineSys::StateMachine* (__cdecl* FFn)(unsigned int, EARS::StateMachineSys::StateMachineParams*), const char* SMName)
{
	using namespace EARS::Framework;
	using namespace EARS::StateMachineSys;
	MemUtils::CallClassMethod<void, SMBuilder*, uint32_t, StateMachine* (__cdecl*)(unsigned int, StateMachineParams*), const char*>(0x04ADC90, this, ClassID, FFn, SMName);
}
