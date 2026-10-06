#include "smbuilder.h"

#include "Platform/MemUtils.h"

// SDK
#include "ears_statemachine/statemachinemanager.h"

void EARS::Framework::SMBuilderState::AddChild(const char* ChildName, bool bEvalParentTrans)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, bool>(0x04ACF10, this, ChildName, bEvalParentTrans);
}

void EARS::Framework::SMBuilderState::AddTransition(const char* DestStateName, uint32_t MsgID)
{
	MemUtils::CallClassMethod<void, EARS::Framework::SMBuilderState*, const char*, uint32_t>(0x04ACC70, this, DestStateName, MsgID);
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

EARS::Framework::SMBuilder::SMBuilder(const char* InTableName, EA::Allocator::IAllocator* InAllocator)
	: m_Allocator(InAllocator)
	, m_States(InAllocator, EA::TagValuePair(0, 0))
	, m_Name(InTableName)
	, m_StateMachineClassID(0)
{
	m_Allocator->AddRef();
	m_StateTableID = StateMachineSys::StateMachineManager::GetInstance()->GetStateTableIDFromName(m_Name);
}

EARS::Framework::SMBuilderState* EARS::Framework::SMBuilder::AddState(const char* StateName, int StateEnum)
{
	using namespace EARS::Framework;
	return MemUtils::CallClassMethod<SMBuilderState*, SMBuilder*, const char*, int>(0x04ADAE0, this, StateName, StateEnum);
}

void EARS::Framework::SMBuilder::CompileAndRegister(uint32_t ClassID, EARS::StateMachineSys::StateMachine* (__cdecl* FFn)(unsigned int, EARS::StateMachineSys::StateMachineParams*), const char* SMName)
{
	using namespace EARS::Framework;
	using namespace EARS::StateMachineSys;
	MemUtils::CallClassMethod<void, SMBuilder*, uint32_t, StateMachine* (__cdecl*)(unsigned int, StateMachineParams*), const char*>(0x04ADC90, this, ClassID, FFn, SMName);
}
