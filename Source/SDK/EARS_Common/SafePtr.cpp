#include "SafePtr.h"

SafePtrBase::~SafePtrBase()
{
	if (m_Obj)
	{
		m_Obj->RemoveSafePtr(this);
	}
}

void SafePtrBase::ClearSafePtr()
{
	if (m_Obj)
	{
		m_Obj->RemoveSafePtr(this);
		m_Obj = nullptr;
	}
}

void SafePtrBase::InitSafePtr(SafeObj* NewObj)
{
	if (m_Obj != NewObj)
	{
		if (m_Obj)
		{
			NewObj->RemoveSafePtr(this);
		}

		m_Obj = NewObj;

		if (m_Obj)
		{
			NewObj->AddSafePtr(this);
		}
	}
}

void SafeObj::AddSafePtr(SafePtrBase* InBase)
{
	InBase->m_Next = m_SafePtrList;
	m_SafePtrList = InBase;
}

void SafeObj::RemoveSafePtr(SafePtrBase* InBase)
{
	assert(m_SafePtrList);
	assert(InBase);

	if (m_SafePtrList == InBase)
	{
		m_SafePtrList = InBase->m_Next;
	}
	else
	{
		SafePtrBase* i = m_SafePtrList;
		for (; i->m_Next != InBase; i = i->m_Next)
		{
			assert(i->m_Next);
		}

		i->m_Next = InBase->m_Next;
	}
}
