#pragma once

// CPP
#include <assert.h>

struct SafeObj;
struct SafePtrBase;

struct SafePtrData
{
protected:

	const SafeObj* m_Obj = nullptr;
	SafePtrBase* m_Next = nullptr;
};

struct SafePtrBase : public SafePtrData
{
};

struct SafeObj
{
private:

	void* __vftable = nullptr /*VFT*/;
	SafePtrBase* m_SafePtrList = nullptr;
};

template <typename T>
struct SafePtr : public SafePtrBase
{
	T* GetPtr() const
	{
		assert(false && "This must be explicitly defined, otherwise you'll crash");
		return nullptr;
	}
};

#include "SafePtr.inl"
